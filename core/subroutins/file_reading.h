#ifndef _CORE__SUBROUTINS__FILE_READING_H_
#define _CORE__SUBROUTINS__FILE_READING_H_

#ifdef BOOST_LIB_USED
  #include <boost/noncopyable.hpp>
#endif  // BOOST_LIB_USED

// #include <exception>
#include <fstream>
#include <string>
#include <vector>

#include <stdint.h>

#ifdef BOOST_LIB_USED
class ReadFile : private boost::noncopyable {
#else
class ReadFile {
  ReadFile(const ReadFile &) = delete;
  ReadFile &operator=(const ReadFile &) = delete;
#endif  // BOOST_LIB_USED
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
