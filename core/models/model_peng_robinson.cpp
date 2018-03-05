#include "model_peng_robinson.h"
#include "models_exceptions.h"
#include "models_math.h"

#include <iostream>
#include <vector>

//================================
// Peng_Robinson ctor
//================================

real_gas_models::Peng_Robinson::Peng_Robinson(modelName mn,
                     std::shared_ptr<constgasparameters> &cgp)
  :modelGeneral::modelGeneral(mn, cgp) {
  modelCoefA_ = 0.45724 * std::pow(parameters_->cgetR(), 2.0) *
    std::pow(parameters_->cgetT_K(), 2.0) / parameters_->cgetP_K();
  modelCoefB_ = 0.0778 * parameters_->cgetR() * parameters_->cgetT_K() /
    parameters_->cgetP_K();
  modelCoefK_ = 0.37464 + 1.54226*parameters_->cgetAcentricFactor() -
               0.26992*std::pow(parameters_->cgetAcentricFactor(), 2.0);
}

//================================
// Peng_Robinson::dynamicflowAccept (visitor accept)
//================================

void real_gas_models::Peng_Robinson::dynamicflowAccept(DerivateFunctor &df) {
  return df.getFunctor(*this);
}

//================================
// Peng_Robinson::isValid
//================================

bool real_gas_models::Peng_Robinson::isValid() const {
  return (parameters_->cgetState() != state_phase::LIQUID);
}

//================================
// Peng_Robinson setters
//================================

void real_gas_models::Peng_Robinson::setVolume(float p, float t) {
  setParameters(getVolume(p, t), p, t);
}

void real_gas_models::Peng_Robinson::setPressure(float v, float t) {
  setParameters(v, getPressure(v, t), t);
}

//================================
// Peng_Robinson::getVolume
//================================

float real_gas_models::Peng_Robinson::getVolume(float p, float t) const {
  if ((p <= 0.0) || (t <= 0.0))
    throw modelExceptions("Peng_Robinson::setVolume bad input p: "
        + std::to_string(p) + " t: " + std::to_string(t));
  float alf = std::pow(1.0 + modelCoefK_*(1.0 -
               t / parameters_->cgetT_K()), 2.0);
  std::vector<float> coef {1.0,
                           modelCoefB_ - parameters_->cgetR()*t/p,
                           (modelCoefA_*alf - 2.0f * modelCoefB_ *
                 parameters_->cgetR()*t)/p-3.0f*modelCoefB_*modelCoefB_,
                           std::pow(modelCoefB_, 3.0f) + (parameters_->cgetR()*
                t *modelCoefB_*modelCoefB_ - modelCoefA_ * alf *modelCoefB_)/p,
                           0.0, 0.0, 0.0};
  try {
    CardanoMethod_HASUNIQROOT(&coef[0], &coef[4]);
  } catch (modelExceptions &e) {
    std::cout << e.what() << std::endl;
    throw modelExceptions(
        "Peng_Robinson::setTemperature calculation error #1");
  }
  if ((coef[4] <= 0.0) || (!std::isfinite(coef[4])))
    throw modelExceptions(
        "Peng_Robinson::setTemperature calculation error #2");
  return coef[4];
}

//================================
// Peng_Robinson::getPressure
//================================

float real_gas_models::Peng_Robinson::getPressure(float v, float t) const {
  if ((v <= 0.0) || (t <= 0.0))
    throw modelExceptions("Peng_Robinson::setPressure bad input v: " +
        std::to_string(v) + " t: " + std::to_string(t));
  const float a = std::pow(1.0 + modelCoefK_ * std::pow(1.0 - std::sqrt(
                                   t / parameters_->cgetT_K()), 2.0), 2.0),
              temp = parameters_->cgetR()*t/(v-modelCoefB_) - a * modelCoefA_ /
                              (v*v+2.0*modelCoefB_*v -modelCoefB_*modelCoefB_);
  if ((temp <= 0.0) || (!std::isfinite(temp)))
    throw modelExceptions(
      "Peng_Robinson::setPressure calculation error (m.b. input data is NaN)");
  return temp;
}

//================================
// Peng_Robinson getters
//================================

float real_gas_models::Peng_Robinson::getCoefA() const {
  return modelCoefA_;
}

float real_gas_models::Peng_Robinson::getCoefB() const {
  return modelCoefB_;
}

float real_gas_models::Peng_Robinson::getCoefK() const {
  return modelCoefK_;
}
