#pragma once

namespace glshader::process
{
    template<typename... Args>
    std::string strfmt(const std::string& format, Args ... args)
    {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
        std::string buf;
        buf.resize(size);
        std::snprintf(buf.data(), size, format.c_str(), args...);
        return buf;
    }

    namespace strings
    {
        constexpr const char* serr_no_endif_else             = "No closing #endif or #else found for if-expression in line %i.";
        constexpr const char* serr_no_endif                  = "No closing #endif found.";
        constexpr const char* serr_invalid_line              = "Invalid line directive, did not find closing \".";
        constexpr const char* serr_invalid_include           = "Include must be in \"...\" or <...>.";
        constexpr const char* serr_file_not_found            = "File not found: %s";
        constexpr const char* serr_eval_end_of_brackets      = "Unexpected end of brackets.";
        constexpr const char* serr_non_matching_argc         = "Macro %s: non-matching argument count.";
        constexpr const char* serr_bad_comment_within_pragma = "Bad comment syntax within a pragma directive.";
		constexpr const char *serr_mismatched_if_else_endif  = "Mismatched #if/#else/#endif";
    }
}
