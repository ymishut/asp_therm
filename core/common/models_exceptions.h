#ifndef SRC_MODELS_EXCEPTIONS_H_
#define SRC_MODELS_EXCEPTIONS_H_

#include <exception>
#include <utility>
#include <string>

namespace real_gas_models {

  //================================
  // modelExceptions
  //================================

  class modelExceptions: public std::exception {
  protected:
    std::string message_;

  public:
    template <class T>
    explicit modelExceptions(T&& m)
      :message_(std::forward<T>(m)) {}

    virtual const char* what() const noexcept {
      return message_.c_str();
    }
    virtual ~modelExceptions() noexcept {}
  };
}  // namespace real_gas_models

#endif  // SRC_MODELS_EXCEPTIONS_H_

