#ifndef _CORE_MODELS__MODEL_REDLICH_KWONG_H_
#define _CORE_MODELS__MODEL_REDLICH_KWONG_H_

#include "gas_parameters/gas_mix_init.h"
#include "model_general.h"

#include <memory>

class Redlich_Kwong2 final: public modelGeneral {
  double modelCoefA_,
         modelCoefB_;

private:
  Redlich_Kwong2(modelName mn, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);
  // Init gas_mix
  Redlich_Kwong2(modelName mn, parameters_mix components,
      binodalpoints bp);

protected:
  void dyn_parameters_update(dyn_parameters &prev,
      const parameters new_state) override;

public:
  static Redlich_Kwong2 *Init(modelName mn, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);
  // Init gas_mix
  static Redlich_Kwong2 *Init(modelName mn, parameters_mix components,
      binodalpoints bp);

  bool IsValid() const;
  void DynamicflowAccept(class DerivateFunctor &df);
  void SetVolume(double p, double t);
  void SetPressure(double v, double t);
  double GetVolume(double p, double t)    const;
  double GetPressure(double v, double t)  const;

  double getCoefA()   const;
  double getCoefB()   const;
};

#endif  // _CORE_MODELS__MODEL_REDLICH_KWONG_H_
