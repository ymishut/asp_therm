#ifndef _CORE__SUBROUTINS__INPUTDATA_BY_FILE_H_
#define _CORE__SUBROUTINS__INPUTDATA_BY_FILE_H_

#include <boost/noncopyable.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <utility>

class InputData: private boost::noncopyable {
public:
  explicit InputData(const std::string &filename);

  std::vector<double> getConstgasparameters();
  std::pair<double, double> getBalloonVF();
  std::vector<double> getBalloonParameters();
  std::string getFunction();
  std::string getEquation();
  std::string getFlowType();
  static void makeExampleInput();

private:
  std::ifstream infile_;
  std::vector<std::string> inputParameters_;
};

#endif  // _CORE__SUBROUTINS__INPUTDATA_BY_FILE_H_

