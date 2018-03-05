#ifndef SRC_FILEREADING_H_
#define SRC_FILEREADING_H_

#include <string>
#include <vector>
#include <exception>
#include <fstream>

#include <boost/noncopyable.hpp>

namespace real_gas_models {
  //================================
  // ReadFile
  //================================

  class ReadFile : private boost::noncopyable {
    size_t currentLine_,
           currentSignificantLine_;
    std::string line_;

    class ReadFileError;

  private:
    void lineProcessing();

  public:
    std::vector<std::string> parseFile(std::ifstream &instream);
  };

  //================================
  // ReadFileError
  //================================

  class ReadFile::ReadFileError: public std::exception {
    std::string message_;

  public:
    ReadFileError(int linnum, std::string message);

    virtual const char* what() const noexcept {
      return message_.c_str();
    }
    ~ReadFileError() noexcept {}
  };
}  // namespace real_gas_models

#endif  // SRC_FILEREADING_H_

