#include "file_reading.h"

#include "models_errors.h"

#include <algorithm>
#include <iostream>

void ReadFile::lineProcessing() {
  if (line_.empty())
    ++currentLine_;
  else if (line_[0] == '#') {
      ++currentLine_;
      line_ = "";
    } else {
      size_t strIter = line_.find('=');
      if (strIter == line_.size()) {
        set_error_code(ERR_FILEIO);
        return;
      }
      line_ = line_.substr(strIter + 1);
      ++currentLine_;
      ++currentSignificantLine_;
    }
}

std::vector<std::string> ReadFile::parseFile(
    std::ifstream &instream) {
  std::vector <std::string> lines;
  reset_error();
  while (true) {
    instream >> std::ws;
    if (!std::getline(instream, line_))
      break;
    lineProcessing();
    if (get_error_code() != ERR_SUCCESS) {
      std::cerr << get_error_message() << std::endl;
      break;
    }
    if (!line_.empty())
      lines.push_back(line_);
  }
  for (auto &x : lines) {
    x.erase(std::remove_if(x.begin(), x.end(), isspace), x.end());
  }
  return lines;
}
