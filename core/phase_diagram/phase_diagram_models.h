#ifndef SRC_PHASE_DIAGRAM_MODELS_H_
#define SRC_PHASE_DIAGRAM_MODELS_H_

#include <vector>

namespace real_gas_models {

  // integrate RK2 function (P = P(v,t)) from cleft to vrigth
  struct lineIntegrateRK2 {
    double operator() (double t, double vLeft, double vRigth, double = 0);
  };

  // calculate volume by RK2 method
  struct initializeRK2 {
    void operator() (std::vector<double>& tempvec, double pi, double t, double = 0);
  };

// same for Reng-Robinson

  struct lineIntegratePR {
    double operator() (double t, double vLeft, double vRigth, double ac);
  };

  struct initializePR {
    void operator() (std::vector<double>& tempvec, double pi, double t, double ac);
  };
}  // namespace real_gas_models
#endif  // SRC_PHASE_DIAGRAM_MODELS_H_

