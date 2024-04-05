/*******************************************************************************
 * File     preprocess.hpp
 * Author   Johannes Braun
 * Created  30.03.2018
 *******************************************************************************/

#pragma once

#include "definition.hpp"
#include "config.hpp"

#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace glshader {
namespace process {

	namespace files = std::filesystem;

    /* The file data after processing. */
    struct processed_file
    {
        files::path file_path;                              /* The original path of the loaded file or the "name" parameter if processed with preprocess_source(...). */
        std::set<files::path> dependencies;                 /* All files included while loading the shader. */
        std::map<std::string, definition_info> definitions; /* All definitions which have been defined in the shader without being undefined afterwards. */
        std::string contents;                               /* The fully processed shader code string. */
        std::vector<std::string> errors;                    /* The number of syntax errors that occurred while preprocessing. */
        bool minified = false;                              /* Generate the smallest possible code footprint. */

        bool valid() const noexcept;                        /* Returns true when the file has been processed successfully, false when there were syntax errors. */
        operator bool() const noexcept;                     /* Returns true when the file has been processed successfully, false when there were syntax errors. */

		void syntax_error(const files::path& file, const int line, const std::string& reason);
    };

	struct pragma_evaluation_context {
	public:
		pragma_evaluation_context(processed_file &file, const std::vector<std::string> &tokens, files::path &current_file, int &current_line,
								  std::stringstream &output);
		~pragma_evaluation_context();

		pragma_evaluation_context(const pragma_evaluation_context &other) = delete;
		pragma_evaluation_context &operator =(const pragma_evaluation_context &other) = delete;

		void syntax_error(const std::string &message) const;

		inline bool is_at_end() const {
			return pos == tokens.end();
		}

		inline const std::string &get() {
			return *pos++;
		}

		std::string expand(const std::string &value) const;

		std::stringstream &output;

	private:
		processed_file &file;
		const std::vector<std::string> &tokens;
		files::path &current_file;
		int &current_line;
		std::vector<std::string>::const_iterator pos;
	};

	class pragma_processor_interface {
	protected:
		pragma_processor_interface() = default;
		~pragma_processor_interface() = default;

	public:
		pragma_processor_interface(const pragma_processor_interface &other) = delete;
		pragma_processor_interface &operator =(const pragma_processor_interface &other) = delete;

		virtual bool process_pragma(pragma_evaluation_context &&context) = 0;
	};

    struct preprocess_info_base {
      std::vector<files::path> include_directories = {}; // A list of include directories to search in when parsing includes.
      std::vector<definition> definitions = {};          // A list of predefined definitions. 
      bool expand_in_macros = false;                     // Recursively expand preprocessor statements if passed as a definition.
      bool do_minify = false;                            // Generate the shortest possible code and leave out #line directives.
      pragma_processor_interface *pragma_processor = nullptr;
    };

    struct preprocess_file_info : preprocess_info_base {
      files::path file_path; // The source file to load.
    };

    struct preprocess_source_info : preprocess_info_base {
      std::string source; // The shader source.
      std::string name;   // The name of the source which will be shown when printing errors.
    };

    /// <summary>
    /// Load and preprocess a shader source text file.
    /// </summary>
    /// <param name="info">Look into the struct <see cref="preprocess_file_info"/> for more info.</param>
    /// <returns>A preprocessed file source. <see cref="processed_file"/></returns>
    processed_file preprocess_file(preprocess_file_info const& info);

    /// <summary>
    /// Load and preprocess a shader source text.
    /// </summary>
    /// <param name="info">Look into the struct <see cref="preprocess_source_info"/> for more info.</param>
    /// <returns>A preprocessed file source. <see cref="processed_file"/></returns>
    processed_file preprocess_source(preprocess_source_info const& info);



    /* A preprocessor state holding include directories and definitions.
    Can be used as a global default for when processing shaders, or as a slightly more flexible way to add definitions and include directories. */
    class state
    {
    public:
        /* Add a persistent definition. */
        void add_definition(const definition& d);
        /* Remove a persistent definition by it's name (without parameters, ect.!). */
        void remove_definition(const std::string& name);

        /* Add a persistent include directory. */
        void add_include_dir(const files::path& dir);
        /* Remove a persistent include directory. */
        void remove_include_dir(const files::path& dir);


        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy),
        and calls the global glsp::preprocess_file function. */
        processed_file preprocess_file(preprocess_file_info info);

        /* Stacks all persistent include directories and definitions onto the ones passed as parameters (Therefore the need to copy),
        and calls the global glsp::preprocess_source function. */
        processed_file preprocess_source(preprocess_source_info info);

    protected:
        std::vector<files::path> _include_directories;
        std::vector<definition> _definitions;
    };
}}
