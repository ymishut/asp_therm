#include "filereading.h"
#include "models_exceptions.h"

#include <iostream>
#include <algorithm>


//================================
// ReadFile::lineProcessing
//================================

void real_gas_models::ReadFile::lineProcessing() {
  if (line_.empty())
    ++currentLine_;
  else if (line_[0] == '#') {
      ++currentLine_;
      line_ = "";
    } else {
      size_t strIter = line_.find('=');
      if (strIter == line_.size())
        throw ReadFileError(currentLine_, line_);
      line_ = line_.substr(strIter + 1);
      ++currentLine_;
      ++currentSignificantLine_;
    }
}

//================================
// ReadFile::parseFile
//================================

std::vector<std::string> real_gas_models::ReadFile::parseFile(
                                      std::ifstream &instream) {
  std::vector <std::string> lines;
  try {
    while (true) {
        instream >> std::ws;
        if (!std::getline(instream, line_))
          break;
        lineProcessing();
        if (!line_.empty())
          lines.push_back(line_);
      }
  } catch (ReadFileError &e) {
    std::cout << e.what()<< std::endl;
    throw modelExceptions(" Bad input data");
  }
  for (auto &x : lines) {
      x.erase(std::remove_if(x.begin(), x.end(), isspace), x.end());
    }
  return lines;
}

//================================
// ReadFileError ctor
//================================

real_gas_models::ReadFile::ReadFileError::ReadFileError(int linnum,
                                                 std::string message)
  :message_(std::to_string(linnum) + ": " + message) {}
