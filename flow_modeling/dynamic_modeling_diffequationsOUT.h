#ifndef SRC_DYNAMIC_MODELING_DIFFEQUATIONSOUT_H_
#define SRC_DYNAMIC_MODELING_DIFFEQUATIONSOUT_H_

#include "dynamic_modeling_diffequations.h"

namespace real_gas_models {
  class idealGas;
  class Redlich_Kwong2;
  class Peng_Robinson;

  //================================
  // OUTdXdtIdGas struct
  //================================

  struct OUTdXdtIdGas: dXdt {
  private:
    idealGas &mg_;

  public:
    OUTdXdtIdGas(idealGas &mg, const balloon &bl, gasparameters &outbl);

    void operator() (const difresult_t &x, difresult_t &dxdt, float = 0);
  };

  //================================
  // OUTdXdtRK2 struct
  //================================

  struct OUTdXdtRK2: dXdt {
  private:
    Redlich_Kwong2 &mg_;
    const float a_, b_;
  
  public:
    OUTdXdtRK2(Redlich_Kwong2 &mg, const balloon &bl, gasparameters &outbl);

    void operator() (const difresult_t &x, difresult_t &dxdt, float = 0);
  };

  //================================
  // OUTdXdtPR struct
  //================================

  struct OUTdXdtPR: dXdt {
  private:
    Peng_Robinson &mg_;
    const float a_, b_, k_;

  public:
    OUTdXdtPR(Peng_Robinson &mg, const balloon &bl, gasparameters &outbl);

    void operator() (const difresult_t &x, difresult_t &dxdt, float = 0);
  };
}

#endif  // SRC_DYNAMIC_MODELING_DIFFEQUATIONSOUT_H_

