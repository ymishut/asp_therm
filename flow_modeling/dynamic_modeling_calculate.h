#ifndef SRC_DYNAMIC_MODELING_CALCULATE_H_
#define SRC_DYNAMIC_MODELING_CALCULATE_H_

#include "dynamic_modeling_models.h"
#include "model_general.h"

#include <utility>

namespace real_gas_models {
  //================================
  // setStartVariables templates
  //================================

  template <class DerivateFunctorType>
  void setStartVariables(float &pm, float &pl, float ac);

  // for inflow outbl>pin
  template <>
  inline void setStartVariables<DerivateFunctorInflow> (float &pm,
                                                        float &pl,
                                                        float ac) {
    (void) pl;
    (void) pm;
    pm *= (1.0 - ac);                                     
  }

  template <>
  inline void setStartVariables<DerivateFunctorOutflow> (float &pm,
                                                         float &pl,
                                                         float ac) {
    std::swap(pl, pm);
    pl *= (1.0 + ac);              
  }
}  // namespace real_gas_models

#endif  // SRC_DYNAMIC_MODELING_CALCULATE_H_

