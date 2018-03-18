#include "load_config.h"

#include "inputdata_by_file.h"
#include "file_reading.h"
#include "models_math.h"
#include "models_exceptions.h"

#include <boost/lexical_cast.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <stdint.h>


double   MODELS_DTIME       = 0.1;
double   MODELS_CALC_ACCUR  = 0.02;
uint32_t MODELS_STEPS       = 100;

bool SetConfigure::loaded = false;

void SetConfigure::loadConf() {
  try {
    ReadFile d;
    auto ifs = std::ifstream("config.cfg");
    std::vector<std::string> info = d.parseFile(ifs);
    MODELS_DTIME      = boost::lexical_cast<double> (info.at(0));
    MODELS_STEPS      = boost::lexical_cast<uint32_t> (info.at(1));
    MODELS_CALC_ACCUR = boost::lexical_cast<double> (info.at(2));
    // check opportunity
    bool cond1 = is_above0(MODELS_DTIME, MODELS_CALC_ACCUR) &&
        (MODELS_CALC_ACCUR < 1.0);
    if (!cond1)
      // assert(0);
      // show some message
      SetConfigure::loaded = false;
      return;
  } catch(std::exception &e) {       // catch bad_cast or modelExc
    std::cerr << e.what() << std::endl;
    throw;
  }
}

void SetConfigure::setConf() {
  // thread_unsafe
  if (!loaded) {
      loaded = true;
      loadConf();
    }
}
