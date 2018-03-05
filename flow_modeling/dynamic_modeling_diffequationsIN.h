#ifndef SRC_DYNAMIC_MODELING_DIFFEQUATIONSIN_H_
#define SRC_DYNAMIC_MODELING_DIFFEQUATIONSIN_H_

#include "dynamic_modeling_diffequations.h"

namespace real_gas_models {
  class idealGas;
  class Redlich_Kwong2;
  class Peng_Robinson;

  //================================
  // INTOdXdtIdGas struct
  //================================

  struct INTOdXdtIdGas: dXdt {
  private:
    idealGas &mg_;

  public:
    INTOdXdtIdGas(idealGas &mg, const balloon &bl, gasparameters &outbl);

    void operator() (const difresult_t &x, difresult_t &dxdt, float = 0);
  };

  //================================
  // INTOdXdtRK2 struct
  //================================

  struct INTOdXdtRK2: dXdt {
  private:
    Redlich_Kwong2 &mg_;
    const float a_, b_;
    float hout_;

  public:
    INTOdXdtRK2(Redlich_Kwong2 &mg, const balloon &bl, gasparameters &outbl);

    void operator() (const difresult_t &x, difresult_t &dxdt, float = 0);
  };

  //================================
  // INTOdXdtPR struct
  //================================

  struct INTOdXdtPR: dXdt {
  private:
    Peng_Robinson &mg_;
    const float a_, b_, k_;
    float hout_;

  public:
    INTOdXdtPR(Peng_Robinson &mg, const balloon &bl, gasparameters &outbl);

    void operator() (const difresult_t &x, difresult_t &dxdt, float = 0);
  };
}  // namespace real_gas_models

#endif  // SRC_DYNAMIC_MODELING_DIFFEQUATIONSIN_H_

