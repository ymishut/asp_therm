#ifndef _CORE__MODELS__MODEL_PENG_ROBINSON_H_
#define _CORE__MODELS__MODEL_PENG_ROBINSON_H_

#include "gas_parameters/gas_mix_init.h"
#include "model_general.h"

#include <memory>

class Peng_Robinson: public modelGeneral {
  double modelCoefA_,
         modelCoefB_,
         modelCoefK_;

private:
  Peng_Robinson(modelName mn, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);
  // Init gas_mix
  Peng_Robinson(modelName mn, parameters_mix components,
      binodalpoints bp);

  double internal_energy_integral(const parameters state);
  double heat_capac_vol_integral(const parameters state);
  double heat_capac_prs_integral(const parameters state);

public:
  static Peng_Robinson *Init(modelName mn, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);
  // Init gas_mix
  static Peng_Robinson *Init(modelName mn, parameters_mix components,
      binodalpoints bp);

  bool IsValid() const;
  void DynamicflowAccept(class DerivateFunctor &df);
  void SetVolume(double p, double t);
  void SetPressure(double v, double t);
  double GetVolume(double p, double t)    const;
  double GetPressure(double v, double t)  const;

  double getCoefA()          const;
  double getCoefB()          const;
  double getCoefK()          const;
};

#endif  // _CORE__MODELS__MODEL_PENG_ROBINSON_H_
