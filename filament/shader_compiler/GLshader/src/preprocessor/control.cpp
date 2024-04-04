#include "control.hpp"
#include <string>

namespace glshader::process::impl::control
{
    std::string line_directive(const files::path& file, int line, processed_file& processed)
    {
      if (processed.minified)
        return "";

      std::string fn = file.filename().string();
      for (auto& c : fn)
        if (c == '\"')
          c = ' ';

      return "\n#line " + std::to_string(line) + " \"" + fn + "\"\n";
	}

    void increment_line(int& current_line, processed_file& processed)
    {
        processed.definitions["__LINE__"] = ++current_line;
    }
}
