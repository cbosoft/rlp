#pragma once

#include <vector>
#include <array>

#include "../colour.hpp"
#include "../exception.hpp"

template<typename T>
class TestRunner {

  private:

    int &counter;
    std::string name;
    bool is_critical;

  protected:

    std::vector<std::array<double, 3>> points;
    std::vector<T> expected_results;

  public:

    TestRunner(int &counter, std::string name, bool is_critical=true) : counter(counter), name(name), is_critical(is_critical) {}

    virtual void run(int testindex) =0;

    void run_each()
    {
      for (size_t i = 0; i < this->expected_results.size(); i++) {
        this->run(i);
      }
    }

    void pass()
    {
      std::cerr << this->name << " " FG_GREEN "PASSED" RESET << std::endl;
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
