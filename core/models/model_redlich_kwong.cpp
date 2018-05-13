#include "model_redlich_kwong.h"

#include "models_math.h"
#include "models_errors.h"

#include <cmath>
// #include <iostream>

#include <assert.h>

void Redlich_Kwong2::set_model_coef() {
  model_coef_a_ = 0.42748*std::pow(parameters_->cgetR(), 2.0) *
      std::pow(parameters_->cgetT_K(), 2.5) / parameters_->cgetP_K();
  model_coef_b_ = 0.08664*parameters_->cgetR()*parameters_->cgetT_K() /
      parameters_->cgetP_K();
}

Redlich_Kwong2::Redlich_Kwong2(modelName mn, parameters prs,
    const_parameters cgp, dyn_parameters dgp, binodalpoints bp)
  : modelGeneral::modelGeneral(mn, prs, cgp, dgp, bp) {
  set_model_coef();
}

Redlich_Kwong2::Redlich_Kwong2(modelName mn, parameters prs,
    parameters_mix components, binodalpoints bp)
  : modelGeneral(mn, prs, components, bp) {
  set_model_coef();
}

Redlich_Kwong2 *Redlich_Kwong2::Init(modelName mn, parameters prs,
    const_parameters cgp, dyn_parameters dgp, binodalpoints bp) {
  reset_error();
  // check const_parameters
 /* if (!is_above0(cgp.acentricfactor, cgp.molecularmass,
      cgp.P_K, cgp.R, cgp.T_K,cgp.V_K)) {
    set_error_code(ERR_INIT | ERR_INIT_ZERO);
    return nullptr;
  } */
  bool is_valid = is_valid_cgp(cgp) && is_valid_dgp(dgp);
  is_valid &= (!is_above0(prs.pressure, prs.temperature, prs.volume));
  if (!is_valid) {
    set_error_code(ERR_INIT_T | ERR_INIT_ZERO);
    return nullptr;
  }
  Redlich_Kwong2 *rk = new Redlich_Kwong2(mn, prs, cgp, dgp, bp);
  // окончательная проверка
  if (rk)
    if (rk->parameters_ == nullptr) {
      set_error_code(ERR_INIT_T);
      delete rk;
      rk = nullptr;
    }
  return rk;
}

Redlich_Kwong2 *Redlich_Kwong2::Init(modelName mn, parameters prs,
    parameters_mix components, binodalpoints bp) {
  // check const_parameters
  reset_error();
  bool is_valid = !components.empty();
  is_valid &= (!is_above0(prs.pressure, prs.temperature, prs.volume));
  if (!is_valid) {
    set_error_code(ERR_INIT_T | ERR_INIT_ZERO | ERR_GAS_MIX);
    return nullptr;
  }
  Redlich_Kwong2 *rk = new Redlich_Kwong2(mn, prs, components, bp);
  if (rk)
    if (rk->parameters_ == nullptr) {
      set_error_code(ERR_INIT_T);
      delete rk;
      rk = nullptr;
    }
  return rk;
 }

//  расчёт смотри в ежедневнике

  // u(p, v, T) = u0 + integrate(....)dv
//   return  u-u0
double Redlich_Kwong2::internal_energy_integral(const parameters state) {
  double ans = 3.0 * model_coef_a_ *
      log(state.volume / (state.volume + model_coef_b_)) /
      (2.0 * sqrt(state.temperature) * model_coef_b_);
  return ans;
}

// cv(p, v, T) = cv0 + integrate(...)dv
//   return cv - cv0
double Redlich_Kwong2::heat_capac_vol_integral(const parameters state) {
  double ans = - 3 * model_coef_a_ *
      log(state.volume / (state.volume + model_coef_b_)) /
      (4.0 * pow(state.temperature, 2.5) * model_coef_b_);
  return ans;
}

// return cp - cv
double Redlich_Kwong2::heat_capac_dif_prs_vol(const parameters state) {
  double R = parameters_->const_params.R,
         T = state.temperature,
         V = state.volume,
         a = model_coef_a_,
         b = model_coef_b_;
  // сначала числитель
  double num = 4.0 * R*R * T*T*T * V*V* (V + b)*(V + b) +
      4.0*(V*V -b*b)*V*R*a*pow(T, 1.5)  +  a*a * (V-b)*(V-b);
  // знаменатель
  double dec = 4.0 * a * (2.0*V*V*V - 3*b*V*V + b*b*b)*pow(T, 1.5) - 
      4.0 * V*V * R * T*T*T*T * (V+b)*(V+b);
  // проверка занменателя на => 0.0
  assert(is_above0(dec) && "Redlich_Kwong2::heat_capac_dif_prs_vol");
  return num / dec;
}

// функция вызывается из класса GasParameters_dyn
void Redlich_Kwong2::update_dyn_params(dyn_parameters &prev_state,
    const parameters new_state) {
  parameters prev_parm = prev_state.parm;
  // internal_energy addition 
  double du  = internal_energy_integral(new_state);
  // heat_capacity_volume addition
  double dcv = heat_capac_vol_integral(new_state);
  // cp - cv
  double dif_c = -new_state.temperature * heat_capac_dif_prs_vol(new_state);
  prev_state.internal_energy += du;
  prev_state.heat_cap_vol    += dcv;
  prev_state.heat_cap_pres   += prev_state.heat_cap_vol + dif_c;
  prev_state.parm = new_state;
  prev_state.Update();
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
    set_error_code(ERR_CALCULATE_T | ERR_CALC_MODEL);
    return 0.0;
  }
  std::vector<double> coef {
      1.0,
      -parameters_->cgetR()*t/p,
      model_coef_a_/(p*std::sqrt(t)) - parameters_->cgetR()*
          t*model_coef_b_/p - model_coef_b_*model_coef_b_,
      -model_coef_a_*model_coef_b_/(p*std::sqrt(t)),
      0.0, 0.0, 0.0
  };
  // Следующая функция заведомо получает валидные
  //   данные,  соответственно должна что-то вернуть
  //   Не будем перегружать код лишними проверками
  CardanoMethod_HASUNIQROOT(&coef[0], &coef[4]);
#ifdef _DEBUG
  if (!is_above0(coef[4])) {
    set_error_code(ERR_CALCULATE_T | ERR_CALC_MODEL);
    return 0.0;
  }
#endif
  return coef[4];
}

double Redlich_Kwong2::GetPressure(double v, double t) const {
  if (!is_above0(v, t)) {
    set_error_code(ERR_CALCULATE_T | ERR_CALC_MODEL);
    return 0.0;
  }
  const double temp = parameters_->cgetR() * t / (v - model_coef_b_) -
      model_coef_a_ / (std::sqrt(t)* v *(v + model_coef_b_));
  return temp;
}

double Redlich_Kwong2::GetCoefficient_a() const {
  return model_coef_a_;
}

double Redlich_Kwong2::GetCoefficient_b() const {
  return model_coef_b_;
}
