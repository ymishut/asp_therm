#ifndef SRC_DYNAMIC_MODELING_DIFFEQUATIONS_H_
#define SRC_DYNAMIC_MODELING_DIFFEQUATIONS_H_

#include "model_general.h"
#include "gas_description_static.h"

namespace real_gas_models {
  //================================
  // balloon struct
  //================================

  struct balloon {
    const float capacity,                // m^3
                nozzleSq;                // m^2

    balloon(const float V, const float F);
  };

  //================================
  // dXdt struct
  //================================

  struct dXdt {
  protected:
    const balloon &bl_;
  // parameters of gas outside the balloon
    gasparameters &outbl_;                                                                      

  // G - [kg/sec]; w - [m/sec]
    void calculateGW(float &G, float &w, const float p_less,
                                         const float p_more,
                                         const float vol_const);

    dXdt(const balloon &bl, gasparameters &outbl);

  // x = { 1/volume, temperature }
    virtual void operator() (const difresult_t &x, difresult_t &dxdt,
                                                        float = 0) = 0;
  public:
    virtual ~dXdt() {}
  };
}  // namespace real_gas_models

#endif  // SRC_DYNAMIC_MODELING_DIFFEQUATIONS_H_

