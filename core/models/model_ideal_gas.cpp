#include "model_ideal_gas.h"
#include "models_exceptions.h"

#include<cmath>

//================================
// idealGas ctor
//================================

real_gas_models::idealGas::idealGas(modelName mn,
          std::shared_ptr<constgasparameters> &cgp)
  :modelGeneral::modelGeneral(mn, cgp) {}

//================================
// idealGas::dynamicflowAccept (visitor accept)
//================================

void real_gas_models::idealGas::dynamicflowAccept(DerivateFunctor &df) {
  df.getFunctor(*this);
}

//================================
// idealGas setters
//================================

void real_gas_models::idealGas::setVolume(float p, float t) {
  setParameters(getVolume(p, t), p, t);
}

void real_gas_models::idealGas::setPressure(float v, float t) {
  setParameters(v, getPressure(v, t), t);
}

void real_gas_models::idealGas::setTemperature(float v, float p) {
  if ((v <= 0.0) || (p <= 0.0))
    throw modelExceptions(
        "idealGas::setPressure bad input v: "
        + std::to_string(v) + " p: " + std::to_string(p));
  float temp = p * v / parameters_->cgetR();
  if ((temp <= 0) || (!std::isfinite(temp)))
    throw modelExceptions(
      "idealGas::setTemperature calculation error (m.b. input data is NaN)");
  setParameters(v, p, temp);
}

//================================
// idealGas getters
//================================

float real_gas_models::idealGas::getVolume(float p, float t) const {
  if ((p <= 0.0) || (t <= 0.0))
    throw modelExceptions("idealGas::setVolume bad input p: "
      + std::to_string(p) + " t: " + std::to_string(t));
  float temp = t * parameters_->cgetR() / p;
  if ((temp <= 0) || (!std::isfinite(temp)))
    throw modelExceptions(
        "idealGas::setVolume calculation error (m.b. input data is NaN)");
  return temp;
}

float real_gas_models::idealGas::getPressure(float v, float t) const {
  if ((v <= 0.0) || (t <= 0.0))
    throw modelExceptions("idealGas::setPressure bad input v: "
      + std::to_string(v) + " t: " + std::to_string(t));
  float temp = t * parameters_->cgetR() / v;
  if ((temp <= 0) || (!std::isfinite(temp)))
    throw modelExceptions(
        "idealGas::setPressure calculation error (m.b. input data is NaN)");
  return temp;
}

//================================
// idealGas isValid
//================================

bool real_gas_models::idealGas::isValid() const {
  return parameters_->cgetState() == state_phase::GAS;
}

