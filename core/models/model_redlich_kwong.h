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
  void update_dyn_params(dyn_parameters &prev_state,
      const parameters new_state) override;

// integrals for calculating u, cv and cv
  double internal_energy_integral(const parameters state);
  double heat_capac_vol_integral(const parameters state);
  double heat_capac_prs_integral(const parameters state);
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
