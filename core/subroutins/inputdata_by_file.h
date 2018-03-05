#ifndef SRC_INPUTDATA_BY_FILE_H_
#define SRC_INPUTDATA_BY_FILE_H_

#include <fstream>
#include <vector>
#include <string>
#include <utility>

#include <boost/noncopyable.hpp>

namespace real_gas_models {
  //================================
  // InputData
  //================================

  class InputData: private boost::noncopyable {
  public:
    explicit InputData(const std::string &filename);

    std::vector<float> getConstgasparameters();
    std::pair<float, float> getBalloonVF();
    std::vector<float> getBalloonParameters();
    std::string getFunction();
    std::string getEquation();
    std::string getFlowType();
    static void makeExampleInput();

  private:
    std::ifstream infile_;
    std::vector<std::string> inputParameters_;
  };
}  // namespace real_gas_models

#endif  // SRC_INPUTDATA_BY_FILE_H_

