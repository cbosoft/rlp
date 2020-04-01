#pragma once

#include <vector>
#include <array>

#include "../colour.hpp"
#include "../exception.hpp"

template<typename I, typename ER>
class TestRunner {

  private:

    int &counter, curidx;
    std::string name;
    bool is_critical;

  protected:

    std::vector<I> input_data;
    std::vector<ER> expected_results;

  public:

    TestRunner(int &counter, std::string name, bool is_critical=true) : counter(counter), name(name), is_critical(is_critical) {}

    virtual void run(I input, ER expected_result) =0;

    void run_each()
    {
      for (size_t i = 0; i < this->expected_results.size(); i++) {
        this->curidx = i;
        this->run(this->input_data[i], this->expected_results[i]);
      }
    }

    void pass()
    {
      std::cerr << this->name << "[" << this->curidx+1 << "/" << this->input_data.size() << "] " FG_GREEN "PASSED" RESET << std::endl;
    }

    void fail(std::string reason)
    {
      this->counter ++;

      std::stringstream ss;
      ss << this->name << " " BG_RED "FAILED" RESET " " << reason;
      if (this->is_critical) {
        throw TestError(ss.str());
      }
      else {
        std::cerr << ss.str() << std::endl;
      }
    }
};
