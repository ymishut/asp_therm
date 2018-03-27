#include "gas_description_static.h"

#include "models_math.h"

#include <array>

#include <assert.h>
//==================================================================
// protected constructors
//==================================================================

GasParameters::GasParameters(double v, double p, double t,
    const const_parameters cgp, dyn_parameters dgp)
  : vpte_(parameters{v, p, t}), const_parameters_(cgp),
    dyn_parameters_(dgp) {}

GasParameters::GasParameters(parameters prs,
    const_parameters cgp, dyn_parameters dgp)
  : vpte_(prs), const_parameters_(cgp), dyn_parameters_(dgp) {}

//==================================================================
// static Init methods
//==================================================================

GasParameters *GasParameters::Init(parameters prs,
    const const_parameters cgp, dyn_parameters dgp) {
  assert(0);
}

GasParameters::~GasParameters() {}

std::ostream &operator<< (std::ostream &outstream,
    const GasParameters &gp) {
  outstream << "v: " << gp.cgetVolume() << " p: " << gp.cgetPressure()
            << " t: " << gp.cgetTemperature() << "\n";
  return outstream;
}

//==================================================================
// const_gasparametrs fields
//==================================================================

double GasParameters::cgetV_K() const {
  return const_parameters_.V_K;
}

double GasParameters::cgetP_K() const {
  return const_parameters_.P_K;
}

double GasParameters::cgetT_K() const {
  return const_parameters_.T_K;
}

double GasParameters::cgetMolecularMass() const {
  return const_parameters_.molecularmass;
}

double GasParameters::cgetR() const {
  return const_parameters_.R;
}

double GasParameters::cgetAcentricFactor() const {
  return const_parameters_.acentricfactor;
}

//==================================================================
// dyn_gasparametrs fields
//==================================================================
double GasParameters::cgetAdiabatic() const {
  return dyn_parameters_.heat_cap_pres / dyn_parameters_.heat_cap_vol;
}

double GasParameters::cgetCV() const {
  return dyn_parameters_.heat_cap_vol;
}

double GasParameters::cgetBeta() const {
  return dyn_parameters_.beta_kr;
}

//==================================================================
// current parametrs of gas
//==================================================================

double GasParameters::cgetVolume() const {
  return vpte_.volume;
}

double GasParameters::cgetPressure() const {
  return vpte_.pressure;
}

double GasParameters::cgetTemperature() const {
  return vpte_.temperature;
}

state_phase GasParameters::cgetState() const {
  return sph_;
}

parameters GasParameters::cgetParameters() const {
  return vpte_;
}

dyn_parameters GasParameters::cgetDynParameters() const {
  return dyn_parameters_;
}

const_parameters GasParameters::cgetConstparameters() const {
  return const_parameters_;
}

// virtual
void GasParameters::csetParameters(double v, double p, double t,
    state_phase sp) {
  vpte_ = {v,p,t};
  sph_  = sp;
}
