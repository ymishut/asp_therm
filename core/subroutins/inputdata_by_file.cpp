#include "inputdata_by_file.h"

#include "file_reading.h"
#include "models_errors.h"

#include <boost/lexical_cast.hpp>

#include <iostream>

#include <stdint.h>

// count of arguments for
namespace {
  const uint32_t CONST_GASP     = 7,
                 BALLOON_VF     = 2,
                 IN_OUT_BALLOON = 6;
}  // namespace

InputData::InputData(const std::string &filename)
  : infile_(filename, std::ios_base::in) {
  if (!infile_.is_open())
    set_error_code(ERR_FILEIO_T);
  try {
    ReadFile d;
    inputParameters_ = (d.parseFile(infile_));
  } catch(modelExceptions &e) {
    set_error_code(ERR_FILEIO_T);
    std::cerr << e.what() << std::endl;
  }
}

std::vector<double> InputData::getConstgasparameters() {
  std::vector<double> vec;
  try {
    for (size_t i = 0; i < CONST_GASP; ++i) {
      vec.push_back(boost::lexical_cast<double>(inputParameters_[i]));
    }
  } catch (boost::bad_lexical_cast &e) {
    std::cerr << e.what() << std::endl;
    set_error_code(ERR_STRING_T);
  }
  return vec;
}

std::pair<double, double> InputData::getBalloonVF() {
  std::pair<double, double> pr;
  try {
    pr = std::make_pair(
         boost::lexical_cast<double>(inputParameters_[CONST_GASP]),
         boost::lexical_cast<double>(inputParameters_[CONST_GASP + 1]));
  } catch (boost::bad_lexical_cast &e) {
    std::cerr << e.what() << std::endl;
    set_error_code(ERR_STRING_T);
  }
  return pr;
}

std::vector<double> InputData::getBalloonParameters() {
  std::vector<double> vec;
  try {
    for (size_t i = CONST_GASP+BALLOON_VF;
        i < CONST_GASP+BALLOON_VF + IN_OUT_BALLOON; ++i) {
        vec.push_back(boost::lexical_cast<double>(inputParameters_[i]));
      }
  } catch (boost::bad_lexical_cast &e) {
    std::cerr << e.what() << std::endl;
    set_error_code(ERR_STRING_T);
  }
  return vec;
}

std::__cxx11::string InputData::getFunction() {
  return inputParameters_[CONST_GASP + BALLOON_VF + IN_OUT_BALLOON];
}

std::__cxx11::string InputData::getEquation() {
  return inputParameters_[CONST_GASP + BALLOON_VF + IN_OUT_BALLOON + 1];
}

std::__cxx11::string InputData::getFlowType() {
  return inputParameters_[CONST_GASP + BALLOON_VF + IN_OUT_BALLOON + 2];
}

void InputData::makeExampleInput() {
  std::fstream f("exampleinput.txt", std::ios_base::out);
  f <<
    "# Parameters of the real gas for dynamic calculation (balloon inflow or outflow)\n"
    "# Dimensions : \n"
    "#     - VOLUME      = [m^3/kg] \n"
    "#     - PRESSURE    = [Pa] \n"
    "#     - TEMPERATURE = [K]  \n"
    "#     - MOLECULAR_M = [kg/mol] \n"
    "#     - ADIABATIC_I = [1] \n"
    "#     - HEAT_CAP_V  = [J/(kg*K)] \n"
    "#     - ACENTRIC_F  = [1] \n"
    "#     - CAPACITY    = [m^3] \n"
    "#     - NOZZLE_SQ   = [m^2] \n"
    "\n"
    "# Constant gas parameters \n"
    " # Parameters in K point \n"
    "  VOLUME      = 0.00617 \n"
    "  PRESSURE    = 4641000 \n"
    "  TEMPERATURE = 190.66 \n"
    "\n"
    "  # Another constants \n"
    "  MOLECULAR_MASS      = 16.043 \n"
    "  ADIABATIC_INDEX     = 1.3 \n"
    "  HEAT_CAPACITY_AT_CONSTANT_VOLUME = 1700 \n"
    "  ACENTRIC_FACTOR     = 0.010 \n"
    "\n"
    "# Parameters of the balloon \n"
    "  CAPACITY      = 28.872 \n"
    "  NOZZLE_SQUARE = 0.000785 \n"
    "\n"
    "# Gas parameters in the balloon \n"
    "  VOLUME      = 0.0 \n"
    "  PRESSURE    = 2.0e5 \n"
    "  TEMPERATURE = 253 \n"
    "\n"
    "# Gas parameters out of the balloon \n"
    "  VOLUME      = 0.0 \n"
    "  PRESSURE    = 2.5e7 \n"
    "  TEMPERATURE = 293 \n"
    "\n"
    "# Set the function to set parameters in/out the balloon (P - p(v,t),V - v(p,t)) \n"
    "  FUNCTION = V \n"
    "\n"
    "# Set the equation of state (IDEAL_GAS, REDLICH_KWONG, PENG_ROBINSON) \n"
    "  EQUATION = PENG_ROBINSON \n"
    "\n"
    "# Type of the flow [ IN, OUT] balloon \n"
    "  FLOW = IN \n";
}
