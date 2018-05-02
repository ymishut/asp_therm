#include "model_peng_robinson.h"

#include "models_errors.h"
#include "models_math.h"

#include <iostream>
#include <vector>

/// =========================================================================
/// Peng_robinson methods
/// =========================================================================
void Peng_Robinson::set_model_coef() {
  modelCoefA_ = 0.45724 * std::pow(parameters_->cgetR(), 2.0) *
      std::pow(parameters_->cgetT_K(), 2.0) / parameters_->cgetP_K();
  modelCoefB_ = 0.0778 * parameters_->cgetR() * parameters_->cgetT_K() /
      parameters_->cgetP_K();
  modelCoefK_ = 0.37464 + 1.54226*parameters_->cgetAcentricFactor() -
      0.26992*std::pow(parameters_->cgetAcentricFactor(), 2.0);
}

Peng_Robinson::Peng_Robinson(modelName mn, parameters prs,
    const_parameters cgp, dyn_parameters dgp, binodalpoints bp)
  : modelGeneral::modelGeneral(mn, prs, cgp, dgp, bp) {
  set_model_coef();
}

Peng_Robinson::Peng_Robinson(modelName mn, parameters prs,
    parameters_mix components, binodalpoints bp) 
    : modelGeneral::modelGeneral(mn, prs, components, bp) {
  set_model_coef();
}

Peng_Robinson *Peng_Robinson::Init(modelName mn, parameters prs,
    const_parameters cgp, dyn_parameters dgp, binodalpoints bp) {
  // check const_parameters
  reset_error();
  bool is_valid = is_valid_cgp(cgp) && is_valid_dgp(dgp);
  is_valid &= (!is_above0(prs.pressure, prs.temperature, prs.volume));
  if (!is_valid) {
    set_error_code(ERR_INIT | ERR_INIT_ZERO);
    return nullptr;
  }
  return new Peng_Robinson(mn, prs, cgp, dgp, bp);
}

Peng_Robinson *Peng_Robinson::Init(modelName mn, parameters prs,
    parameters_mix components, binodalpoints bp) {
  reset_error();
  // для проверки установленных доль
  bool is_valid = !components.empty();
  is_valid &= (!is_above0(prs.pressure, prs.temperature, prs.volume));
  if (!is_valid) {
    set_error_code(ERR_INIT | ERR_INIT_ZERO | ERR_GAS_MIX);
    return nullptr;
  }
  return new Peng_Robinson(mn, prs, components, bp);
}

//  расчёт смотри в ежедневнике

  // u(p, v, T) = u0 + integrate(....)dv
//   return  u-u0
double Peng_Robinson::internal_energy_integral(const parameters state) {
  return ans;
}

// cv(p, v, T) = cv0 + integrate(...)dv
//   return cv - cv0
double Peng_Robinson::heat_capac_vol_integral(const parameters state) {
  return ans;
}

// return cp - cv
double Peng_Robinson::heat_capac_dif_prs_vol(const parameters state) {
  assert(0);
  double R = parameters_->const_params.R,
         T = state.temperature,
         V = state.volume,
         a = modelCoefA_,
         b = modelCoefB_;
  // сначала числитель
  double num = 4.0 * R*R * T*T*T * V*V* (V + b)*(V + b) +
      4.0*(V*V -b*b)*V*R*a*pow(T, 1.5)  +  a*a * (V-b)*(V-b);
  // знаменатель
  double dec = 4.0 * a * (2.0*V*V*V - 3*b*V*V + b*b*b)*pow(T, 1.5) - 
      4.0 * V*V * R * T*T*T*T * (V+b)*(V+b);
  // проверка занменателя на => 0.0
  assert(is_above0(dec) && "Peng_Robinson::heat_capac_dif_prs_vol");
  return num / dec;
}

// функция вызывается из класса GasParameters_dyn
void Peng_Robinson::update_dyn_params(dyn_parameters &prev_state,
    const parameters new_state) {
  parameters prev_parm = prev_state.parm;
  // internal_energy addition 
  double du  = internal_energy_integral(new_state);
  // heat_capacity_volume addition
  double dcv = heat_capac_vol_integral(new_state);
  // cp - cv
  double dif_c = -new_state.temperature * heat_capac_dif_prs_vol(new_state);
  prev_state.interval_energy += du;
  prev_state.heat_cap_vol    += dcv;
  prev_state.heat_cap_pres   += prev_state.heat_cap_vol + dif_c;
  prev_state.parm = new_state;
  prev_state.Update();
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
