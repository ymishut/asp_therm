#include "load_config.h"
#include "inputdata_by_file.h"
#include "models_exceptions.h"
#include "filereading.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

float  MODELS_DTIME      = 0.1;
float  MODELS_CALC_ACCUR = 0.02;
size_t MODELS_STEPS      = 100;

bool real_gas_models::SetConfigure::loaded = false;

//================================
// SetConfigure::loadConf
//================================

void real_gas_models::SetConfigure::loadConf() {
  try {
    ReadFile d;
    auto ifs = std::ifstream("config.cfg");
    std::vector<std::string> info = d.parseFile(ifs);
    MODELS_DTIME      = boost::lexical_cast<double> (info.at(0));
    MODELS_STEPS      = boost::lexical_cast<size_t> (info.at(1));
    MODELS_CALC_ACCUR = boost::lexical_cast<double> (info.at(2));
    // check opportunity
    auto f = [](float a) {
        return (std::isfinite(a) && (a > 0.0));};
    bool cond1 = f(MODELS_DTIME) && f(MODELS_CALC_ACCUR)
                                 && (MODELS_CALC_ACCUR < 1.0);
    if (!cond1)
      throw modelExceptions(
          "loadConf: bad input data for DTIME or CALC_ACCUR");
  } catch(std::exception &e) {       // catch bad_cast or modelExc
    std::cout << e.what() << std::endl;
    throw;
  }
}

//================================
// SetConfigure::setConf
//================================

void real_gas_models::SetConfigure::setConf() {
  if (!loaded) {
      loadConf();
      loaded = true;
    }
}
