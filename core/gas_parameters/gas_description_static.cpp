#include "gas_description_static.h"
#include "models_exceptions.h"

//================================
// gasparameters ctor
//================================

real_gas_models::gasparameters::gasparameters(float v, float p, float t,
                                  std::shared_ptr<constgasparameters> cgp)
  :vpte_(v, p, t), constparameters_(cgp) {
  if (cgp == nullptr)
    throw modelExceptions(
        "gasparameters_static::gasparameters_static\
         gets nullptr to constgasparameters");
}

//================================
// gasparameter ctor
//================================

real_gas_models::gasparameters::gasparameters(real_gas_models::parameters prs,
                                        std::shared_ptr<constgasparameters> cgp)
  :vpte_(prs), constparameters_(cgp) {
  if (cgp == nullptr)
    throw modelExceptions(
        "gasparameters_static::gasparameters_static\
         gets nullptr to constgasparameters");
}

//================================
// gasparameter operator <<
//================================

std::ostream &real_gas_models::operator<< (std::ostream &outstream,
                                           const gasparameters &gp) {
  outstream << "v: " << gp.cgetVolume() << " p: " << gp.cgetPressure()
            << " t: " << gp.cgetTemperature() << "\n";
  return outstream;
}

//================================
// gasparameter getters
//================================

float real_gas_models::gasparameters::cgetV_K() const {
  return constparameters_->V_K;
}

float real_gas_models::gasparameters::cgetP_K() const {
  return constparameters_->P_K;
}

float real_gas_models::gasparameters::cgetT_K() const {
  return constparameters_->T_K;
}

float real_gas_models::gasparameters::cgetMolecularMass() const {
  return constparameters_->molecularmass;
}

float real_gas_models::gasparameters::cgetAdiabatic() const {
  return constparameters_->adiabatic_index;
}

float real_gas_models::gasparameters::cgetCV() const {
  return constparameters_->cv;
}

float real_gas_models::gasparameters::cgetBeta() const {
  return constparameters_->beta_kr;
}

float real_gas_models::gasparameters::cgetR() const {
  return constparameters_->R;
}

float real_gas_models::gasparameters::cgetAcentricFactor() const {
  return constparameters_->acentricfactor;
}

float real_gas_models::gasparameters::cgetVolume() const {
  return vpte_.volume;
}

float real_gas_models::gasparameters::cgetPressure() const {
  return vpte_.pressure;
}

float real_gas_models::gasparameters::cgetTemperature() const {
  return vpte_.temperature;
}

real_gas_models::state_phase
real_gas_models::gasparameters::cgetState() const {
  return sph_;
}

real_gas_models::parameters
real_gas_models::gasparameters::cgetParameters() const {
  return parameters(vpte_);
}

std::shared_ptr<real_gas_models::constgasparameters>
real_gas_models::gasparameters::cgetConstparameters() const {
  return constparameters_;
}

//================================
// gasparameter::csetParameters
//================================

void real_gas_models::gasparameters::csetParameters(float v, float p, float t,
                                                               state_phase sp) {
  vpte_.volume        = v;
  vpte_.pressure      = p;
  vpte_.temperature   = t;
  sph_                = sp;
}
