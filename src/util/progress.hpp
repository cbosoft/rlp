#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <unistd.h>
#include <sys/ioctl.h>
#include "../util/colour.hpp"


static std::vector<std::string> colours = {
  FG_BLUE,
  FG_YELLOW,
  FG_GREEN,
  FG_CYAN,
  FG_MAGENTA
};

class ProgressBar {

  private:

    std::vector<std::string> variables;
    std::vector<int> maximums;
    std::vector<int> values;

  public:

    ProgressBar() {};
    ProgressBar(std::vector<std::string> variables, std::vector<int> maximums)
      : variables(variables), maximums(maximums)
    {
      for (auto v : variables) {
        (void) v;
        std::cerr << std::endl;
      }
    }

    void set_values(std::vector<int> values)
    {
      this->values = values;
    }

    void draw()
    {
      struct winsize size;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
      size_t cols = size.ws_col;

      // move cursor up number of variables
      std::cerr << "\r\033[" << this->variables.size() << "A";

      // for each variable, draw bar
      for (size_t i = 0; i < this->variables.size(); i++) {
        size_t perc = this->maximums[i] ? (this->values[i]*cols)/this->maximums[i] : 0;

        if (perc > cols)
          perc = cols;

        std::cerr << "\033[K" << colours[i];
        for (size_t j = 0; j < perc; j++) {
          std::cerr << "█";
        }
        std::cerr << RESET << "\r" << this->variables[i] << colours[i] << "▐" << RESET << "\r\n";
      }

    }




};
