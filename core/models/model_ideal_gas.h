#ifndef _CORE__MODELS__MODEL_IDEAL_GAS_H_
#define _CORE__MODELS__MODEL_IDEAL_GAS_H_

#include <memory>

#include "model_general.h"

class IdealGas final: public modelGeneral {
private:
  IdealGas(modelName mn, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);

  void update_dyn_params(dyn_parameters &prev_state,
      const parameters new_state) override;

public:
  static IdealGas *Init(modelName mn, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);

  bool IsValid() const override;
  void DynamicflowAccept(DerivateFunctor &df) override;
//  void setTemperature(double v, double p);

  void SetVolume(double p, double t)      override;
  void SetPressure(double v, double t)    override;
  double GetVolume(double p, double t)    const override;
  double GetPressure(double v, double t)  const override;
};

#endif  // _CORE__MODELS__MODEL_IDEAL_GAS_H_
