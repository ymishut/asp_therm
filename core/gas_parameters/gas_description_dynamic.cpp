#include "gas_description_dynamic.h"
#include "models_exceptions.h"

#include <cmath>
#include <utility>

//================================
// gasparameters_dynamic ctor
//================================

real_gas_models::gasparameters_dynamic::gasparameters_dynamic(
                                    float v, float p, float t,
                      std::shared_ptr<constgasparameters> cgp)
  :current_vpte_(v, p, t), previous_vpte_(v, p, t), constparameters_(cgp) {
  bool isValid = true;
  for (const auto &x : {v, p, t})
    isValid &= ((x > 0.0) && std::isfinite(x));
  if (!isValid)
    throw modelExceptions("gasparameters_dynamic::gasparameters_dynamic:\
                           bad input parameters v,p or t");
  if (cgp == nullptr)
    throw modelExceptions(
        "gasparameters_dynamic::gasparameters_dynamic gets nullptr");
}

//================================
// gasparameters_dynamic ctor
//================================

real_gas_models::gasparameters_dynamic::gasparameters_dynamic(parameters prs,
                                       std::shared_ptr<constgasparameters> cgp)
  :current_vpte_(prs), previous_vpte_(prs), constparameters_(cgp) {
  bool isValid = true;
  for (const auto &x : {prs.volume, prs.pressure, prs.temperature})
    isValid &= ((x > 0.0) && std::isfinite(x));
  if (!isValid)
    throw modelExceptions(
        "gasparameters_dynamic::gasparameters_dynamic:\
         bad input parameters v, p or t");
  if (cgp == nullptr)
    throw modelExceptions(
        "gasparameters_dynamic::gasparameters_dynamic gets nullptr");
}

//================================
// potentials operator <<
//================================

std::ostream &real_gas_models::operator<< (std::ostream &outstream,
                                              const potentials &pt) {
  outstream << "h: " << pt.enthalpy << " u: " << pt.internalenergy
            << " Gibbs: " << pt.Gibbsfree << "\n Hermholtz: "
            << pt.Hermholtz_free << " Landau-Grand: " << pt.LandauGrand << "\n";
  return outstream;
}

//================================
// gasparameters_dynamic  getters
//================================

float real_gas_models::gasparameters_dynamic::cgetV_K() const {
  return constparameters_->V_K;
}

float real_gas_models::gasparameters_dynamic::cgetP_K() const {
  return constparameters_->P_K;
}

float real_gas_models::gasparameters_dynamic::cgetT_K() const {
  return constparameters_->T_K;
}

float real_gas_models::gasparameters_dynamic::cgetMolecularMass() const {
  return constparameters_->molecularmass;
}

float real_gas_models::gasparameters_dynamic::cgetAdiabatic() const {
  return constparameters_->adiabatic_index;
}

float real_gas_models::gasparameters_dynamic::cgetCV() const {
  return constparameters_->cv;
}

float real_gas_models::gasparameters_dynamic::cgetBeta() const {
  return constparameters_->beta_kr;
}

float real_gas_models::gasparameters_dynamic::cgetR() const {
  return constparameters_->R;
}

float real_gas_models::gasparameters_dynamic::cgetAcentricFactor() const {
  return constparameters_->acentricfactor;
}

float real_gas_models::gasparameters_dynamic::cgetVolume() const {
  return current_vpte_.volume;
}

float real_gas_models::gasparameters_dynamic::cgetPressure() const {
  return current_vpte_.pressure;
}

float real_gas_models::gasparameters_dynamic::cgetTemperature() const {
  return current_vpte_.temperature;
}

real_gas_models::state_phase
real_gas_models::gasparameters_dynamic::cgetState() const {
  return sph_;
}

real_gas_models::parameters
real_gas_models::gasparameters_dynamic::cgetParameters() const {
  return parameters(current_vpte_);
}

std::shared_ptr<real_gas_models::constgasparameters>
real_gas_models::gasparameters_dynamic::cgetConstparameters() const {
  return constparameters_;
}

//================================
// gasparameters_dynamic csetParameters
//================================

void real_gas_models::gasparameters_dynamic::csetParameters(float v, float p,
                                                     float t, state_phase sp) {
  std::swap(current_vpte_, previous_vpte_);
  current_vpte_.volume       = v;
  current_vpte_.pressure     = p;
  current_vpte_.temperature  = t;
  sph_                       = sp;
  // updatePotentials();
}

//================================
// gasparameters_dynamic operator <<
//================================

std::ostream &real_gas_models::operator<< (std::ostream &outstream,
                                   const gasparameters_dynamic &gp) {
  outstream << "v: " << gp.cgetVolume() << " p: " << gp.cgetPressure()
                    << " t: " << gp.cgetTemperature() << "\n";
  return outstream;
}
