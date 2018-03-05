#include "inputdata_by_file.h"
#include "models_exceptions.h"
#include "filereading.h"

#include <iostream>

#include <boost/lexical_cast.hpp>

// count of arguments for
namespace {
  const size_t CONST_GASP     = 7,
               BALLOON_VF     = 2,
               IN_OUT_BALLOON = 6;
}  // namespace

//================================
// InputData ctor
//================================

real_gas_models::InputData::InputData(const std::__cxx11::string &filename)
  :infile_(filename, std::ios_base::in) {
  if (!infile_.is_open()) {
      throw modelExceptions("Cannot open file to read init data: " + filename);
    }
  try {
    ReadFile d;
    inputParameters_ = (d.parseFile(infile_));
  } catch(modelExceptions &e) {
    std::cout << e.what() << std::endl;
  }
}

//================================
// InputData::getConstgasparameters
//================================

std::vector<float> real_gas_models::InputData::getConstgasparameters() {
  std::vector<float> vec;
  try {
    for (size_t i = 0; i < CONST_GASP; ++i) {
        vec.push_back(boost::lexical_cast<double>(inputParameters_[i]));
      }
  } catch (boost::bad_lexical_cast &e) {
    std::cout << e.what();
    throw modelExceptions(
        "Cannot convert input data to constgasparameters ctor args");
  }
  return vec;
}

//================================
// InputData::getBalloonVF
//================================

std::pair<float, float> real_gas_models::InputData::getBalloonVF() {
  std::pair<float, float> pr;
  try {
    pr = std::make_pair(
          boost::lexical_cast<double>(inputParameters_[CONST_GASP]),
          boost::lexical_cast<double>(inputParameters_[CONST_GASP + 1])
          );
  } catch (boost::bad_lexical_cast &e) {
    std::cout << e.what() << std::endl;
    throw modelExceptions("Cannot convert input data to balloon ctor args");
  }
  return pr;
}

//================================
// InputData::getBalloonParameters
//================================

std::vector<float> real_gas_models::InputData::getBalloonParameters() {
  std::vector<float> vec;
  try {
    for (size_t i = CONST_GASP+BALLOON_VF;
        i < CONST_GASP+BALLOON_VF + IN_OUT_BALLOON; ++i) {
        vec.push_back(boost::lexical_cast<double>(inputParameters_[i]));
      }
  } catch (boost::bad_lexical_cast &e) {
    std::cout << e.what() << std::endl;
    throw modelExceptions(
        "Cannot convert input data to [in & out] balloon ctor args");
  }
  return vec;
}

//================================
// InputData::getFunction
//================================

std::__cxx11::string real_gas_models::InputData::getFunction() {
  return inputParameters_[CONST_GASP + BALLOON_VF + IN_OUT_BALLOON];
}

//================================
// InputData::getEquation
//================================

std::__cxx11::string real_gas_models::InputData::getEquation() {
  return inputParameters_[CONST_GASP + BALLOON_VF + IN_OUT_BALLOON + 1];
}

//================================
// InputData::getFlowType
//================================

std::__cxx11::string real_gas_models::InputData::getFlowType() {
  return inputParameters_[CONST_GASP + BALLOON_VF + IN_OUT_BALLOON + 2];
}

//================================
// InputData::makeExampleInput
//================================

void real_gas_models::InputData::makeExampleInput() {
  std::fstream f("exampleinput.txt", std::ios_base::out);
  f <<
    "# Parameters of the real gas for dynamic calculation (balloon inflow or outflow)\
    \
    # Dimensions : \
    #     - VOLUME      = [m^3/kg] \
    #     - PRESSURE    = [Pa] \
    #     - TEMPERATURE = [K]  \
    #     - MOLECULAR_M = [kg/mol] \
    #     - ADIABATIC_I = [1] \
    #     - HEAT_CAP_V  = [J/(kg*K)] \
    #     - ACENTRIC_F  = [1] \
    #     - CAPACITY    = [m^3] \
    #     - NOZZLE_SQ   = [m^2] \
    \
    # Constant gas parameters \
      # Parameters in K point \
      VOLUME      = 0.00617 \
      PRESSURE    = 4641000 \
      TEMPERATURE = 190.66 \
    \
      # Another constants \
      MOLECULAR_MASS      = 16.043 \
      ADIABATIC_INDEX     = 1.3 \
      HEAT_CAPACITY_AT_CONSTANT_VOLUME = 1700 \
      ACENTRIC_FACTOR     = 0.010 \
    \
    # Parameters of the balloon \
      CAPACITY      = 28.872 \
      NOZZLE_SQUARE = 0.000785 \
    \
    # Gas parameters in the balloon \
      VOLUME      = 0.0 \
      PRESSURE    = 2.0e5 \
      TEMPERATURE = 253 \
    \
    # Gas parameters out of the balloon \
      VOLUME      = 0.0 \
      PRESSURE    = 2.5e7 \
      TEMPERATURE = 293 \
     \
    # Set the function to set parameters in/out the balloon (P - p(v,t),V - v(p,t)) \
      FUNCTION = V \
    \
    # Set the equation of state (IDEAL_GAS, REDLICH_KWONG, PENG_ROBINSON) \
      EQUATION = PENG_ROBINSON \
    \
    # Type of the flow [ IN, OUT] balloon \
      FLOW = IN \
";
}


