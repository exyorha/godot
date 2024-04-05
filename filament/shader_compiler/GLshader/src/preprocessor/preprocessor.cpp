#include <glsp/preprocess.hpp>
#include <glsp/config.hpp>

/* Impl */
#include "../strings.hpp"
#include "eval.hpp"
#include "control.hpp"
#include "classify.hpp"
#include "skip.hpp"
#include "macro.hpp"

#include <fstream>
#include <iterator>
#include <cassert>
#include <sstream>
#include <cstring>
#include <algorithm>

namespace glshader::process
{
    namespace cls = impl::classify;
    namespace ctrl = impl::control;
    namespace skip = impl::skip;
    namespace macro = impl::macro;

    inline std::string minify(std::string const& original) {
      std::string minified;
      //preallocate a bit more than needed.
      minified.reserve(original.size());

      enum class state {
        normal,
        space,
        preprocessor
      } current_state = state::normal;

      bool alphanumeric = false;

      for (char c : original) {
        switch (current_state) {
        case state::space:
          if (!(cls::is_space(&c) || cls::is_newline(&c)) && !(c == '\n' || c == '\r'))
          {
            current_state = state::normal;

            if (alphanumeric && !cls::is_operator(&c))
              minified.push_back(' ');
          }
        case state::normal:
          if (cls::is_space(&c) || cls::is_newline(&c))
            current_state = state::space;
          else if (c == '#') {
            if (!minified.empty() && !cls::is_newline(&minified.back()))
              minified.push_back('\n');
            current_state = state::preprocessor;
            minified.push_back(c);
          }
          else {
            alphanumeric = !cls::is_operator(&c);
            minified.push_back(c);
          }
          break;

        case state::preprocessor:
          if (c == '\n' || c == '\r')
            current_state = state::space;
          minified.push_back(c);
          break;
        }
      }
      minified.shrink_to_fit();
      return minified;
    }

	static bool skip_over_failed_conditional_with_nesting(const char *&text_ptr,
														  processed_file &processed,
														  files::path &current_file,
														  int &current_line,
														  bool also_accept_else,
														  std::stringstream &result) {
		int nesting = 0;
		while (true)
		{
			text_ptr = skip::over_comments(text_ptr, current_file, current_line, processed, result);

			if (cls::is_newline(text_ptr))
			{
				ctrl::increment_line(current_line, processed);
			}
			else if (cls::is_directive(text_ptr))
			{
				if (cls::is_token_equal(skip::space(text_ptr + 1), "if", 2, true, false))
					++nesting;
				else if (cls::is_token_equal(skip::space(text_ptr + 1), "endif", 5) ||
					     (also_accept_else && cls::is_token_equal(skip::space(text_ptr + 1), "else", 4)) ||
					     (also_accept_else && cls::is_token_equal(skip::space(text_ptr + 1), "elif", 4)))
				{
					if (nesting == 0)
						break;
					else nesting--;
				}
			}
			else if (cls::is_eof(text_ptr))
			{
				processed.syntax_error(current_file, current_line, strings::serr_no_endif);
				return false;
			}

			++text_ptr;
		}

		return true;
	}

    void process_impl(const files::path& file_path, const std::string& contents, const std::vector<files::path>& include_directories,
        processed_file& processed, std::set<files::path>& unique_includes,
        std::stringstream& result, bool expand_in_macros, pragma_processor_interface *pragma_processor)
    {
        std::vector<bool> accept_else_directive;

        const char* text_ptr = contents.data();
        files::path current_file = file_path;
        processed.definitions["__FILE__"] = current_file.string();
        int current_line = 1;
        std::string curr = current_file.filename().string();
        std::replace(curr.begin(), curr.end(), '\\', '/');

        // There is no way you could put a macro starting from the first character of the shader.
        // Set to true if the current text_ptr may point to the start of a macro name.
        bool enable_macro = false;

        while (!cls::is_eof(text_ptr))
        {
            text_ptr = skip::over_comments(text_ptr, current_file, current_line, processed, result);
            if (cls::is_eof(text_ptr))
                break;

            if (cls::is_newline(text_ptr))
            {
                ctrl::increment_line(current_line, processed);
                result << '\n';
                ++text_ptr;
                enable_macro = true;
            }
            else if (enable_macro && macro::is_macro(text_ptr, processed))
            {
              if (expand_in_macros) {
                std::stringstream tempstream;
                tempstream << '\n';
                tempstream << ctrl::line_directive(current_file, current_line, processed);
                tempstream << macro::expand(text_ptr, text_ptr, current_file, current_line, processed);
                tempstream << ctrl::line_directive(current_file, current_line + 1, processed);
                tempstream << '\n';
                process_impl(file_path, tempstream.str(), include_directories, processed, unique_includes, result, expand_in_macros,
							 pragma_processor);
              }
              else
              {
                result << ctrl::line_directive(current_file, current_line, processed);
                result << macro::expand(text_ptr, text_ptr, current_file, current_line, processed);
                result << ctrl::line_directive(current_file, current_line + 1, processed);
              }
              ++text_ptr;
            }
            else if (cls::is_directive(text_ptr, current_line != 1))
            {
                const auto directive_name = skip::space(text_ptr + 1);
				if (cls::is_token_equal(directive_name, "pragma", 6))
                {
					text_ptr = directive_name;
					std::vector<std::string> tokens;

					while(!impl::classify::is_newline(text_ptr) && !impl::classify::is_eof(text_ptr)) {
						while(impl::classify::is_space(text_ptr) || *text_ptr == '/' || *text_ptr == '\\') {
							text_ptr = skip::space(text_ptr);
							auto comment_ptr = skip::over_comments(text_ptr, current_file, current_line, processed, result);

							if(text_ptr == comment_ptr && *text_ptr == '/') {
                                processed.syntax_error(current_file, current_line, strings::serr_bad_comment_within_pragma);
                                return;
							}

							text_ptr = comment_ptr;

							if(*text_ptr == '\\') {
								text_ptr++;

								if(impl::classify::is_newline(text_ptr)) {
									text_ptr++;
									ctrl::increment_line(current_line, processed);
								}
							}
						}

						auto end_of_token = skip::to_next_space(text_ptr, '/');

						if(text_ptr != directive_name) {
							tokens.emplace_back(text_ptr, end_of_token);
						}

						text_ptr = end_of_token;
					}

                    if (!tokens.empty() && tokens.front() == "once")
                    {
                        unique_includes.emplace(current_file);

						result << ctrl::line_directive(current_file, current_line, processed);

                    } else if(pragma_processor && pragma_processor->process_pragma(pragma_evaluation_context(processed, tokens, current_file, current_line,
																											 result))) {
						/*
						 * The pragma directive has been processed externally, and it shouldn't be emitted into the source code.
						 */

						result << ctrl::line_directive(current_file, current_line, processed);

					} else if(!tokens.empty()) {

						result << ctrl::line_directive(current_file, current_line, processed);

                        result << "#pragma";
						for(const auto &token: tokens) {
							result << " " << token;
						}
						result << "\n";
                    }
                }
                else if (cls::is_token_equal(directive_name, "define", 6))
                {
                    text_ptr = skip::to_next_token(directive_name);
                    const auto name_begin = text_ptr;
                    while (!cls::is_space(text_ptr) && !cls::is_newline(text_ptr) && (*text_ptr != '(' || (*text_ptr == '(' && *skip::space(text_ptr+1)==')')))
                        ++text_ptr;

                    if (const auto space_skipped = skip::space(text_ptr);
                    cls::is_space(text_ptr) && !cls::is_newline(space_skipped) && !cls::is_comment(space_skipped))
                    {
                        // macro without params
                        auto value_end = space_skipped;
                        std::stringstream val;
                        while (!cls::is_newline(value_end) && !cls::is_eof(value_end) && !cls::is_comment(value_end))
                        {
                            if (*value_end != '\\')
                                val << *value_end;
                            else
                            {
                                value_end = skip::to_endline(value_end) + 1;
                                ctrl::increment_line(current_line, processed);
                            }
                            ++value_end;
                        }

                        processed.definitions[{name_begin, text_ptr}] = val.str();

                        text_ptr = value_end;
                    }
                    else if (cls::is_newline(text_ptr) || cls::is_newline(space_skipped) || cls::is_comment(space_skipped))
                    {
                        // define without value
                        processed.definitions[{name_begin, text_ptr}];
                    }
                    else if (*space_skipped == '(')
                    {
                        // macro with params
                        const auto name_end = text_ptr;
                        const auto params_begin = space_skipped + 1;
                        while (*text_ptr != ')')
                            ++text_ptr;
                        const auto params_end = text_ptr;

                        // macro without params
                        auto value_end = skip::space(params_end + 1);
                        std::stringstream definition_stream;
                        while (!(cls::is_newline(value_end) && *(value_end - 1) != '\\'))
                        {
                            if (*value_end != '\\')
                                definition_stream << *value_end;
                            else
                            {
                                ctrl::increment_line(current_line, processed);
                            }
                            if (cls::is_eof(value_end))
                                break;
                            ++value_end;
                        }
                        --current_line;

                        std::string parameter;
                        std::vector<std::string> parameters;
                        std::stringstream param_stream({ skip::space(params_begin), params_end });

                        while (std::getline(param_stream, parameter, ','))
                        {
                            const auto param_begin = skip::space(parameter.data());
                            parameters.push_back({ param_begin, skip::to_next_space(param_begin) });
                        }

                        while (param_stream >> parameter)
                        {
                            parameters.push_back(parameter);

                            if (param_stream.peek() == ',' || param_stream.peek() == ' ')
                                param_stream.ignore();
                        }

                        processed.definitions[{name_begin, name_end}] ={ std::move(parameters), definition_stream.str() };

                        text_ptr = value_end;
                    }

                    result << ctrl::line_directive(current_file, current_line, processed);
                }
                else if (cls::is_token_equal(directive_name, "undef", 5))
                {
                    text_ptr = skip::to_next_token(directive_name);
                    const auto begin = text_ptr;
                    while (!cls::is_eof(text_ptr) && !cls::is_space(text_ptr) && !cls::is_newline(text_ptr))
                        ++text_ptr;

                    processed.definitions.erase({ begin, text_ptr });
                }
                else if (const auto elif = cls::is_token_equal(directive_name, "elif", 4); cls::is_token_equal(directive_name, "if", 2, true, false) || (elif))
                {
                    text_ptr = skip::to_next_token(directive_name);
                    const auto value_begin = text_ptr;
                    bool enable_nl = true;
                    while (!enable_nl || !(cls::is_newline(text_ptr) || cls::is_eof(text_ptr)))
                    {
                        enable_nl = true;
                        if (*text_ptr == '\\')
                            enable_nl = false;
                        ++text_ptr;
                    }

                    bool evaluated;
                    if (cls::is_token_equal(directive_name, "ifdef", 5))
                        evaluated =  macro::is_defined({ value_begin, text_ptr }, processed);
                    else if (cls::is_token_equal(directive_name, "ifndef", 6))
                        evaluated = !macro::is_defined({ value_begin, text_ptr }, processed);
                    else if (elif && !accept_else_directive.back())
                        evaluated = false;
                    else
                    {
                        // Simple IF
                        std::stringstream line;
                        for (auto i = value_begin; i != text_ptr; ++i)
                        {
                            if (memcmp(i, "//", 2) == 0)
                            {
                                break;
                            }
                            if (memcmp(i, "/*", 2) == 0)
                            {
                                while (memcmp(i, "*/", 2) != 0)
                                    ++i;
                                ++i;
                            }
                            else if (cls::is_token_equal(i, "defined", 7))
                            {
                                while (*i != '(')
                                    ++i;
                                const auto defined_macro_begin = i;
                                while (*i != ')')
                                    ++i;

                                line << (macro::is_defined(std::string(defined_macro_begin + 1, i), processed) ? '1' : '0');
                            }
                            else
                                line << *i;
                        }

                        std::string fully_expanded_conditional;

                        auto line_str = line.str();

						const char *unexpanded_ptr = line_str.data();
						while(*unexpanded_ptr != 0 && !cls::is_newline(unexpanded_ptr)) {
							auto begin = unexpanded_ptr;

							auto str = macro::expand(unexpanded_ptr, unexpanded_ptr, current_file, current_line, processed);
							fully_expanded_conditional.append(str);

							if(unexpanded_ptr == begin) {
								break;
							}
						}

                        evaluated = impl::operation::eval(fully_expanded_conditional.data(), static_cast<int>(fully_expanded_conditional.length()), current_file, current_line, processed);
                    }

                    if (evaluated)
                    {
                        text_ptr = skip::to_endline(text_ptr);
                        if (elif) {
							if(accept_else_directive.empty()) {
								processed.syntax_error(current_file, current_line, strings::serr_mismatched_if_else_endif);
								return;
							}
                            accept_else_directive.back() = false;
						} else
                            accept_else_directive.push_back(false);
                    }
                    else
                    {
                        if (!elif)
                            accept_else_directive.push_back(true);


						if(!skip_over_failed_conditional_with_nesting(text_ptr, processed, current_file, current_line, true, result)) {
							return;
						}

                    }
                }
                else if (cls::is_token_equal(directive_name, "else", 4))
                {
					if(accept_else_directive.empty()) {
						processed.syntax_error(current_file, current_line, strings::serr_mismatched_if_else_endif);
						return;
					}

                    if (accept_else_directive.back())
                    {
                        text_ptr = skip::to_endline(text_ptr);
                    }
                    else
                    {
						if(!skip_over_failed_conditional_with_nesting(text_ptr, processed, current_file, current_line, false, result)) {
						   return;
						}

                    }
                }
                else if (cls::is_token_equal(directive_name, "endif", 5))
                {
					if(accept_else_directive.empty()) {
						processed.syntax_error(current_file, current_line, strings::serr_mismatched_if_else_endif);
						return;
					}

					accept_else_directive.pop_back();

					text_ptr = skip::to_endline(directive_name);
                    result << ctrl::line_directive(current_file, current_line, processed);
                }
                else if (cls::is_token_equal(directive_name, "line", 4))
                {
                    text_ptr = skip::to_next_token(directive_name);
                    const auto line_nr_end = skip::to_next_space(text_ptr);

                    result << "#line ";

                    int new_line_number = 0;
                    for (auto i = text_ptr; (i != line_nr_end) && (new_line_number *= 10) != -1; ++i)
                        new_line_number += *i - '0';

                    result << new_line_number << " ";

                    text_ptr = skip::space(line_nr_end);
                    if (*text_ptr == '\"')
                    {
                        if (const auto file_name_end = skip::to_next_space(text_ptr) - 1; *file_name_end == '\"')
                        {
                            current_file = files::path(std::string(text_ptr + 1, file_name_end));
                            processed.definitions["__FILE__"] = current_file.string();
                            result << '\"' << current_file.string() << '\"' << '\n';
                        }
                        else
                        {
                            processed.syntax_error(current_file, current_line, strings::serr_invalid_line);

                            current_file = files::path(std::string(text_ptr + 1, file_name_end));
                            processed.definitions["__FILE__"] = current_file.string();
                            result << '\"' << current_file.string() << '\"' << '\n';
                        }
                    }
                    text_ptr = skip::to_endline(text_ptr);

                    ctrl::increment_line(current_line = new_line_number - 1, processed);
                }
                else if (cls::is_token_equal(directive_name, "error", 5))
                {
                    const auto begin = skip::to_next_token(directive_name);
                    processed.syntax_error(current_file, current_line, std::string(begin, skip::to_endline(begin)).c_str());
                    return;
                }
                else if (cls::is_token_equal(directive_name, "include", 7))
                {
                    auto include_begin = skip::to_next_token(text_ptr);
                    auto include_filename = macro::expand(include_begin, include_begin, current_file, current_line, processed);

                    if ((include_filename.front() != '\"' && include_filename.back() != '\"') && (include_filename.
                        front() != '<' && include_filename.back() != '>'))
                    {
                        processed.syntax_error(current_file, current_line, strings::serr_invalid_include);
                        return;
                    }
                    files::path file ={ std::string(include_filename.begin() + 1, include_filename.end() - 1) };

                    bool found_file = false;
                    for (auto&& directory : include_directories)
                    {
                        if (exists(directory / file))
                        {
                            found_file = true;
                            file = directory / file;
                        }
                    }

                    if (!found_file)
                    {
                        file = file_path.parent_path() / file;
                    }

                    if (!files::exists(file))
                    {
                        processed.syntax_error(current_file, current_line, strfmt(strings::serr_file_not_found, std::string(include_filename.begin() + 1, include_filename.end() - 1).c_str()));
                        return;
                    }

                    if (unique_includes.count(file) == 0)
                    {
                        result << ctrl::line_directive(current_file, current_line, processed);
                        result << ctrl::line_directive(file, 1, processed);
                        processed.dependencies.emplace(file);

                        std::ifstream root_file(file, std::ios::in);
                        std::string contents(std::istreambuf_iterator<char>{root_file}, std::istreambuf_iterator<char>{});
                        process_impl(file, contents, include_directories, processed, unique_includes, result, expand_in_macros,
									 pragma_processor);
                    }
                    text_ptr = skip::to_endline(include_begin);

                    result << ctrl::line_directive(current_file, current_line, processed);
                }
                else
                {
                    if (!cls::is_name_char(text_ptr))
                        enable_macro = true;
                    else
                        enable_macro = false;
                    result << *text_ptr;
                    ++text_ptr;
                }
            }
            else
            {
                if (!cls::is_name_char(text_ptr))
                    enable_macro = true;
                else
                    enable_macro = false;

                result << *text_ptr;
                ++text_ptr;
            }
        }

		if(!accept_else_directive.empty()) {
			processed.syntax_error(current_file, current_line, strings::serr_mismatched_if_else_endif);
		}
    }

    processed_file preprocess_file(preprocess_file_info const& info)
    {
      if (!exists(info.file_path))
      {
        processed_file processed;
		processed.syntax_error("Preprocessor", 0, strfmt(strings::serr_file_not_found, info.file_path.string().c_str()));
        return processed;
      }

      preprocess_source_info source_info;
      static_cast<preprocess_info_base&>(source_info) = static_cast<preprocess_info_base>(info);
      std::ifstream root_file(info.file_path, std::ios::in);
      source_info.source = std::string(std::istreambuf_iterator<char>{root_file}, std::istreambuf_iterator<char>{});
      source_info.name = info.file_path.string();
      return preprocess_source(source_info);
    }

    processed_file preprocess_source(preprocess_source_info const& info)
    {
      processed_file processed;
      processed.file_path = info.name;
      processed.minified = info.do_minify;

      for (auto&& definition : info.definitions)
        processed.definitions[definition.name] = definition.info;

      std::stringstream result;
      std::set<files::path> unique_includes;
      unique_includes.emplace(info.name);
      process_impl(info.name, info.source, info.include_directories, processed, unique_includes, result, info.expand_in_macros, info.pragma_processor);

      processed.contents = result.str();

      if (info.do_minify)
      {
        processed.contents = minify(processed.contents);
      }

      return processed;
    }

    void state::add_definition(const definition& d)
    {
        _definitions.push_back(d);
    }

    void state::remove_definition(const std::string& name)
    {
        if (const auto it = std::find_if(_definitions.begin(), _definitions.end(), [&name](const definition& def) {return def.name == name; }); it != _definitions.end())
            _definitions.erase(it);
    }

    void state::add_include_dir(const files::path& dir)
    {
        _include_directories.push_back(dir);
    }

    void state::remove_include_dir(const files::path& dir)
    {
        if (const auto it = std::find(_include_directories.begin(), _include_directories.end(), dir); it != _include_directories.end())
            _include_directories.erase(it);
    }

    processed_file state::preprocess_file(preprocess_file_info info)
    {
      info.include_directories.insert(info.include_directories.begin(), _include_directories.begin(), _include_directories.end());
      info.definitions.insert(info.definitions.begin(), _definitions.begin(), _definitions.end());
      return glsp::preprocess_file(info);
    }

    processed_file state::preprocess_source(preprocess_source_info info)
    {
      info.include_directories.insert(info.include_directories.begin(), _include_directories.begin(), _include_directories.end());
      info.definitions.insert(info.definitions.begin(), _definitions.begin(), _definitions.end());
      return glsp::preprocess_source(info);
    }

    bool processed_file::valid() const noexcept
    {
        return errors.empty();
    }

    processed_file::operator bool() const noexcept
    {
        return valid();
    }

    void processed_file::syntax_error(const files::path &file, const int line, const std::string &reason) {
        errors.emplace_back(file.string() + ":" + std::to_string(line) + ": " + reason);
    }

    pragma_evaluation_context::pragma_evaluation_context(processed_file &file, const std::vector<std::string> &tokens, files::path &current_file,
														 int &current_line,  std::stringstream &output) :
		output(output), file(file), tokens(tokens), current_file(current_file), current_line(current_line), pos(tokens.begin()) {

	}

	pragma_evaluation_context::~pragma_evaluation_context() = default;

	void pragma_evaluation_context::syntax_error(const std::string &message) const {
		file.syntax_error(current_file, current_line, message);
	}
}
