#pragma once

#include <vector>
#include <array>
#include <iomanip>

#include "../util/colour.hpp"
#include "../util/exception.hpp"

template<typename I, typename ER>
class TestRunner {

  private:

    int curidx;
    std::string name;

  protected:

    bool is_quiet;
    std::vector<I> input_data;
    std::vector<ER> expected_results;

  public:

    TestRunner(std::string name, bool is_quiet) : name(name), is_quiet(is_quiet) {}

    virtual void run(I input, ER expected_result) =0;

    void run_each()
    {

      if (this->input_data.size() != this->expected_results.size()) {
        throw TestError(Formatter() << "Malformed test! Input data size (" << this->input_data.size() 
            << ") does not match expected results size (" << this->expected_results.size() << ").");
      }

      for (size_t i = 0; i < this->expected_results.size(); i++) {
        this->curidx = i;
        this->run(this->input_data[i], this->expected_results[i]);
      }
    }

    void pass()
    {
      if (not this->is_quiet)
        std::cerr << this->name << "[" << this->curidx+1 << "/" << this->input_data.size() << "] " FG_GREEN "PASSED" RESET << std::endl;
    }

    void fail(std::string reason)
    {
      throw TestError(Formatter() << this->name << " " BG_RED "FAILED" RESET " " << reason);
    }
};
