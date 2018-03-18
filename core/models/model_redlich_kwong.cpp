#include "model_redlich_kwong.h"

#include "models_math.h"
#include "models_errors.h"

#include <iostream>

Redlich_Kwong2::Redlich_Kwong2(modelName mn, const_parameters cgp,
    dyn_parameters dgp, binodalpoints bp)
  : modelGeneral::modelGeneral(mn, cgp, dgp, bp) {
  modelCoefA_ = 0.42748*std::pow(parameters_->cgetR(), 2.0) *
      std::pow(parameters_->cgetT_K(), 2.5) / parameters_->cgetP_K();
  modelCoefB_ = 0.08664*parameters_->cgetR()*parameters_->cgetT_K() /
      parameters_->cgetP_K();
}

Redlich_Kwong2::Redlich_Kwong2(modelName mn, parameters_mix components,
    binodalpoints bp) {}

Redlich_Kwong2 *Redlich_Kwong2::Init(modelName mn, const_parameters cgp,
    dyn_parameters dgp, binodalpoints bp) {
  // check const_parameters
  if (!is_above0(cgp.acentricfactor, cgp.molecularmass,
      cgp.P_K, cgp.R, cgp.T_K,cgp.V_K)) {
    set_error_code(ERR_INIT | ERR_INIT_ZERO);
    return nullptr;
  }
  return new Redlich_Kwong2(mn, cgp, dgp, bp);
}

// visitor
void Redlich_Kwong2::DynamicflowAccept(DerivateFunctor &df) {
  df.getFunctor(*this);
}

bool Redlich_Kwong2::IsValid() const {
  return (parameters_->cgetPressure()/parameters_->cgetP_K() <
      0.5*parameters_->cgetTemperature()/parameters_->cgetT_K());
}

void Redlich_Kwong2::SetVolume(double p, double t) {
  setParameters(GetVolume(p, t), p, t);
}

void Redlich_Kwong2::SetPressure(double v, double t) {
  setParameters(v, GetPressure(v, t), t);
}

double Redlich_Kwong2::GetVolume(double p, double t) const {
  if (!is_above0(p, t)) {
    set_error_code(ERR_CALCULATE | ERR_CALC_MODEL);
    return 0.0;
  }
  std::vector<double> coef {
      1.0,
      -parameters_->cgetR()*t/p,
      modelCoefA_/(p*std::sqrt(t)) - parameters_->cgetR()*
          t*modelCoefB_/p - modelCoefB_*modelCoefB_,
      -modelCoefA_*modelCoefB_/(p*std::sqrt(t)),
      0.0, 0.0, 0.0
  };
  // Следующая функция заведомо получает валидные
  //   данные,  соответственно должна что-то вернуть
  //   Не будем перегружать код лишними проверками
  CardanoMethod_HASUNIQROOT(&coef[0], &coef[4]);
#ifdef _DEBUG
  if (!is_above0(coef[4])) {
    set_error_code(ERR_CALCULATE | ERR_CALC_MODEL);
    return 0.0;
  }
#endif
  return coef[4];
}

double Redlich_Kwong2::GetPressure(double v, double t) const {
  if (!is_above0(v, t)) {
    set_error_code(ERR_CALCULATE | ERR_CALC_MODEL);
    return 0.0;
  }
  const double temp = parameters_->cgetR() * t / (v - modelCoefB_) -
      modelCoefA_ / (std::sqrt(t)* v *(v + modelCoefB_));
  return temp;
}

double Redlich_Kwong2::getCoefA() const {
  return modelCoefA_;
}

double Redlich_Kwong2::getCoefB() const {
  return modelCoefB_;
}
