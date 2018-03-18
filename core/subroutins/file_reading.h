#ifndef _CORE__SUBROUTINS__FILE_READING_H_
#define _CORE__SUBROUTINS__FILE_READING_H_

#include <string>
#include <vector>
#include <exception>
#include <fstream>

#include <boost/noncopyable.hpp>

#include <stdint.h>

class ReadFile : private boost::noncopyable {
  uint32_t currentLine_,
           currentSignificantLine_;
  std::string line_;

  class ReadFileError;

private:
  void lineProcessing();

public:
  std::vector<std::string> parseFile(std::ifstream &instream);
};

/*
class ReadFile::ReadFileError: public std::exception {
  std::string message_;

public:
  ReadFileError(int linnum, std::string message);

  virtual const char* what() const noexcept {
    return message_.c_str();
  }
  ~ReadFileError() noexcept {}
};
*/
#endif  // _CORE__SUBROUTINS__FILE_READING_H_
