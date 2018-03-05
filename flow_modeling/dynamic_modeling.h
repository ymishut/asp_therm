#ifndef SRC_DYNAMIC_MODELING_H_
#define SRC_DYNAMIC_MODELING_H_

#include <iostream>
#include <functional>
#include <string>
#include <memory>

#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

#include "model_general.h"
#include "models_output.h"
#include "dynamic_modeling_diffequations.h"

/// difresult_t  is boost::array<float, 2>
/// difEquat_t   is std::function <void (const difresult_t &x,
//                               difresult_t &dxdt, double t)>

namespace real_gas_models {
  class GasDynamic;
  class DerivateFunctorInflow;
  class DerivateFunctorOutflow;
  class gasparameters;

  //================================
  // balloonFlowDynamic
  //================================

  class balloonFlowDynamic {
    friend class GasDynamic;
    typedef boost::numeric::odeint::runge_kutta4<difresult_t> stepper_t;

  private:
    std::shared_ptr<modelGeneral> calculateModel_;
    gasparametersConverter calculateModelConverter_;
    gasparameters &outbl_;
    const balloon bl_;
    static float accuracy_;

  private:
    balloonFlowDynamic(const std::shared_ptr<modelGeneral> &spmg,
          gasparameters &outballoon, const float V, const float F);

    void updatePress(float &pm, float &pl, DerivateFunctorInflow);

    void updatePress(float &pm, float &pl, DerivateFunctorOutflow);

  public:
    template <class DerivateFunctorType>
    void calculateFlow(const float dtime, const size_t outRate,
                                           formatted_output &out);
  };
}
#endif  // SRC_DYNAMIC_MODELING_H_

