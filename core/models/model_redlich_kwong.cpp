#include "model_redlich_kwong.h"
#include "models_math.h"
#include "models_exceptions.h"

#include <iostream>

//================================
// Redlich_Kwong2 ctor
//================================

real_gas_models::Redlich_Kwong2::Redlich_Kwong2(modelName mn,
                      std::shared_ptr<constgasparameters> &cgp)
  :modelGeneral::modelGeneral(mn, cgp) {
  modelCoefA_ = 0.42748*std::pow(parameters_->cgetR(), 2.0) *
    std::pow(parameters_->cgetT_K(), 2.5) / parameters_->cgetP_K();
  modelCoefB_ = 0.08664*parameters_->cgetR()*parameters_->cgetT_K() /
    parameters_->cgetP_K();
}

//================================
// Redlich_Kwong2::dynamicflowAccept (visitor accept)
//================================

void real_gas_models::Redlich_Kwong2::dynamicflowAccept(DerivateFunctor &df) {
  df.getFunctor(*this);
}

//================================
// Redlich_Kwong2::isValid
//================================

bool real_gas_models::Redlich_Kwong2::isValid() const {
  return (parameters_->cgetPressure()/parameters_->cgetP_K() <
      0.5*parameters_->cgetTemperature()/parameters_->cgetT_K());
}

//================================
// Redlich_Kwong2 setters
//================================

void real_gas_models::Redlich_Kwong2::setVolume(float p, float t) {
  setParameters(getVolume(p, t), p, t);
}

void real_gas_models::Redlich_Kwong2::setPressure(float v, float t) {
  setParameters(v, getPressure(v, t), t);
}

//================================
// Redlich_Kwong2::getVolume
//================================

float real_gas_models::Redlich_Kwong2::getVolume(float p, float t) const {
  if ((p <= 0.0) || (t <= 0.0))
    throw modelExceptions("Redlich_Kwong2::setVolume bad input p: " +
        std::to_string(p) + " t: " + std::to_string(t));
  std::vector<float> coef {1.0,
                           -parameters_->cgetR()*t/p,
                           modelCoefA_/(p*std::sqrt(t))-parameters_->cgetR()*
                                     t*modelCoefB_/p-modelCoefB_*modelCoefB_,
                           -modelCoefA_*modelCoefB_/(p*std::sqrt(t)),
                           0.0, 0.0, 0.0};
  try {
    CardanoMethod_HASUNIQROOT(&coef[0], &coef[4]);
  } catch (modelExceptions &e) {
    std::cout << e.what() << std::endl;
  }
  if ((coef[4] <= 0.0) || (!std::isfinite(coef[4])))
    throw modelExceptions(
      "Redlich_Kwong::setTemperature calculation error");
  return coef[4];
}

//================================
// Redlich_Kwong2::getPressure
//================================

float real_gas_models::Redlich_Kwong2::getPressure(float v, float t) const {
  if ((v <= 0.0) || (t <= 0.0))
    throw modelExceptions("Redlich_Kwong2::setPressure bad input v: " +
        std::to_string(v) + " t: " + std::to_string(t));
  const float temp    = parameters_->cgetR() * t / (v - modelCoefB_) -
    modelCoefA_ / (std::sqrt(t)* v *(v + modelCoefB_));
  if ((temp <= 0.0) || (!std::isfinite(temp)))
    throw modelExceptions(
      "Redlich_Kwong::setTemperature calculation error");
  return temp;
}

//================================
// Redlich_Kwong2 getters
//================================

float real_gas_models::Redlich_Kwong2::getCoefA() const {
  return modelCoefA_;
}

float real_gas_models::Redlich_Kwong2::getCoefB() const {
  return modelCoefB_;
}

