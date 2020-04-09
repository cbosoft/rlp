#pragma once
#include <exception>
#include <string>
#include <iostream>
#include <cstring>

#include "colour.hpp"
#include "formatter.hpp"



class Exception : public virtual std::exception {
  private:

    std::string detail = "";
    std::string type = "";
    std::string cause = "";
    bool cause_set;
    bool use_errno;

  public:
    Exception(std::string detail, std::string type, std::string cause) 
    { 
      this->detail = detail; 
      this->type = type; 
      this->cause = cause;
      this->cause_set = cause.size() > 0;
      this->use_errno = false;
    }

    Exception(std::string detail, std::string type) 
    { 
      this->detail = detail; 
      this->type = type; 
      this->cause_set = false;
      this->use_errno = false;
    }

    Exception(std::string detail, std::string type, bool use_errno)
    {
      this->detail = detail;
      this->type = type;
      this->cause_set = false;
      this->use_errno = use_errno;
    }

    void pretty_print() const
    {
      std::cerr << BOLD << BG_RED << this->type << RESET << ": " << this->detail;
      
      if (use_errno) {
        std::cerr << "(" << errno << ": " << strerror(errno) << ")";
      }

      if (this->cause_set) {
        std::cerr << this->cause;
      }

      std::cerr << std::endl;
    }

    const char *what() const noexcept override
    {
      std::stringstream ss;
      ss << BOLD << BG_RED << this->type << RESET << ": " << this->detail;
      
      if (use_errno) {
        ss << "(" << errno << ": " << strerror(errno) << ")";
      }

      if (this->cause_set) {
        ss << this->cause;
      }

      std::string s = ss.str();
      const char *c = strdup(s.c_str());
      return c;
    }
};





#define EXCEPTION(N,S)\
  class N : virtual public Exception {\
    public:\
           N(std::string detail, std::string cause) : Exception(detail, S, cause) { }\
           N(std::string detail) : Exception(detail, S) { }\
           N(std::string detail, bool use_errno) : Exception(detail, S, use_errno) { }\
  };

EXCEPTION(ArgumentError, "ArgumentError");
EXCEPTION(NameError, "NameError");
EXCEPTION(TypeError, "TypeError");
EXCEPTION(NotImplementedError, "NotImplementedError");
EXCEPTION(AuthorError, "AuthorError");
EXCEPTION(IOError, "IOError");
EXCEPTION(MathError, "MathError");
EXCEPTION(IndexError, "IndexError");
EXCEPTION(SettleError, "SettleError");
EXCEPTION(IntersectionError, "IntersectionError");
EXCEPTION(RecursionError, "RecursionError");
EXCEPTION(TestError, "TestError");



EXCEPTION(AssertError, "AssertError");

inline void assert(bool expression, std::string detail)
{
  if (!expression)
    throw AssertError(detail);
}
