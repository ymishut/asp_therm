#include "model_peng_robinson.h"

#include "models_errors.h"
#include "models_math.h"

#include <iostream>
#include <vector>

/// =========================================================================
/// Peng_robinson methods
/// =========================================================================

Peng_Robinson::Peng_Robinson(modelName mn,
    const_parameters cgp, dyn_parameters dgp, binodalpoints bp)
  : modelGeneral::modelGeneral(mn, cgp, dgp, bp) {
  modelCoefA_ = 0.45724 * std::pow(parameters_->cgetR(), 2.0) *
      std::pow(parameters_->cgetT_K(), 2.0) / parameters_->cgetP_K();
  modelCoefB_ = 0.0778 * parameters_->cgetR() * parameters_->cgetT_K() /
      parameters_->cgetP_K();
  modelCoefK_ = 0.37464 + 1.54226*parameters_->cgetAcentricFactor() -
      0.26992*std::pow(parameters_->cgetAcentricFactor(), 2.0);
}

Peng_Robinson::Peng_Robinson(modelName mn, parameters_mix components,
    binodalpoints bp) {

}

Peng_Robinson *Peng_Robinson::Init(modelName mn, const_parameters cgp,
    dyn_parameters dgp, binodalpoints bp) {
  // check const_parameters
  if (!is_valid_cgp(cgp))
    return nullptr;
  if (!is_valid_dgp(dgp))
    return nullptr;
  return new Peng_Robinson(mn, cgp, dgp, bp);
}

Peng_Robinson *Peng_Robinson::Init(modelName mn, parameters_mix components,
      binodalpoints bp) {
  reset_error();
  // для проверки установленных доль
  double percents = 0.0;
  for (const auto &x : components) {
    if (!is_above0(x.first)) {
      set_error_code(ERR_INIT);
      return nullptr;
    }
    percents += x.first;
    if (!is_valid_cgp(x.second.first))
      return nullptr;
    if (!is_valid_dgp(x.second.second))
      return nullptr;
  }
  if (std::abs(0.99 - percents) > GAS_MIX_PERCENT_EPS) {
    set_error_code(ERR_CALCULATE | ERR_CALC_GAS_P);
    return nullptr;
  }
  return new Peng_Robinson(mn, components, bp);
}

void Peng_Robinson::DynamicflowAccept(DerivateFunctor &df) {
  return df.getFunctor(*this);
}

bool Peng_Robinson::IsValid() const {
  return (parameters_->cgetState() != state_phase::LIQUID);
}

void Peng_Robinson::SetVolume(double p, double t) {
  setParameters(GetVolume(p, t), p, t);
}

void Peng_Robinson::SetPressure(double v, double t) {
  setParameters(v, GetPressure(v, t), t);
}

double Peng_Robinson::GetVolume(double p, double t) const {
  if (!is_above0(p, t)) {
    set_error_code(ERR_CALCULATE | ERR_CALC_MODEL);
    return 0.0;
  }
  double alf = std::pow(1.0 + modelCoefK_*(1.0 -
      t / parameters_->cgetT_K()), 2.0);
  std::vector<double> coef {
      1.0,
      modelCoefB_ - parameters_->cgetR()*t/p,
      (modelCoefA_*alf - 2.0f * modelCoefB_ *
          parameters_->cgetR()*t)/p-3.0f*modelCoefB_*modelCoefB_,
      std::pow(modelCoefB_, 3.0f) + (parameters_->cgetR()*
          t *modelCoefB_*modelCoefB_ - modelCoefA_ * alf *modelCoefB_)/p,
      0.0, 0.0, 0.0};
  CardanoMethod_HASUNIQROOT(&coef[0], &coef[4]);
#ifdef _DEBUG
  if (!is_above0(coef[4])) {
    set_error_code(ERR_CALCULATE | ERR_CALC_MODEL);
    return 0.0;
  }
#endif
  return coef[4];
}

double Peng_Robinson::GetPressure(double v, double t) const {
  if (!is_above0(v, t)) {
    set_error_code(ERR_CALCULATE | ERR_CALC_MODEL);
    return 0.0;
  }
  const double a    = std::pow(1.0 + modelCoefK_ * std::pow(1.0 -
      std::sqrt(t / parameters_->cgetT_K()), 2.0), 2.0),
               temp = parameters_->cgetR()*t/(v-modelCoefB_) -
      a * modelCoefA_ / (v*v+2.0*modelCoefB_*v -modelCoefB_*modelCoefB_);
  return temp;
}

double Peng_Robinson::getCoefA() const {
  return modelCoefA_;
}

double Peng_Robinson::getCoefB() const {
  return modelCoefB_;
}

double Peng_Robinson::getCoefK() const {
  return modelCoefK_;
}
