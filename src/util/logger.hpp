#pragma once

#include <string>
#include <fstream>

#include "exception.hpp"

class Logger {

  private:

    const char *log_file_path;
    int verbosity;
    bool file_logging;

  public:

    Logger()
      : log_file_path(nullptr), verbosity(1), file_logging(false)
    {  }
    Logger(int verbosity)
      : log_file_path(nullptr), verbosity(verbosity), file_logging(false)
    {  }
    Logger(const char *log_file_path, int verbosity)
      : log_file_path(log_file_path), verbosity(verbosity), file_logging(log_file_path != nullptr)
    {  }

    void log(std::string s, int minimum_verbosity=1)
    {
      if (this->verbosity >= minimum_verbosity) {
        std::cerr << s;
      }

      if (this->file_logging) {
        std::ofstream of(this->log_file_path);

        if (of.fail())
          throw IOError("Error opening log file!", true);

        of << s << std::endl;
      }
    }
};
