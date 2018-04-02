#include "gas_mix_init.h"

#include "models_errors.h"

#include <cmath>
#include <vector>

#include <assert.h>

/*
static void add_part_const_parameters(const_parameters *res,
    const double part_per, const const_parameters &part_val) {
  assert(0);
  // см комментарий ниже
  res->molecularmass *= part_per * part_val.molecularmass;
  res->T_K *= part_per * part_val.T_K;
  res->V_K *= part_per * part_val.V_K;
  res->P_K *= part_per * part_val.P_K;
}

std::pair<const_parameters *, dyn_parameters *>
    get_parameters_of_mix(parameters_mix cgp_mix) {
  assert(0);
  // Такой подход неверен, так как фактор ацентричности и индекс
  //   адиабаты не адитивны, и среднее арифмитическое этих величин
  //   не очень
  reset_error();
  if (cgp_mix.empty()) {
    set_error_code(ERR_INIT | ERR_INIT_ZERO);
    return {nullptr, nullptr};
  }
  const_parameters *cgp_p = nullptr;
  dyn_parameters   *dgp_p = nullptr;
}
*/

GasParameters_mix_dyn::GasParameters_mix_dyn(parameters prs,
    const_parameters cgp, dyn_parameters dgp, parameters_mix components,
    dyn_params_update update_f)
  : GasParameters(prs, cgp, dgp), components_(components),
    update_f_(update_f) {}
  // DEVELOP
  //   23_03_2018
  /*   В общем, после долгих раздумий решено:
   *     для статичных расчётов это всё инициализируется по
   *     средним значениям.
   *   В динамике всё пересчитывается по формулам
   *     используемой МОДЕЛИ!
   *   Т.о. для динамики пересчитывается:
   *     -- Ср и Сv по формулам из "Начал тд"
   *     -- Индекс адиабаты по Ср/Сv
   *    НЕ ПЕРЕСЧИТЫВАЮТСЯ И ОБНУЛЕНЫ ВООБЩЕ
   *     -- фактор ацентричности -он не адитивен (мне кажется)
   *          и ладно, его использование ограничено
   */
GasParameters_mix_dyn *GasParameters_mix_dyn::Init(
    parameters prs, parameters_mix components, dyn_params_update update_f) {
  if (components.empty() || update_f == nullptr) {
    set_error_code(ERR_INIT | ERR_INIT_NULLP | ERR_GAS_MIX);
    return nullptr;
  }
// РАСЧИТАЕМ СРЕДНИЕ КОНСТАНТНЫЕ ПАРАМЕТРЫ
  reset_error();
  // количество параметров и их значения для критической точки
  const int32_t critical_pt_meas = 3;
  std::array<double, critical_pt_meas> critical_pt_val = {0.0, 0.0, 0.0};
  // АДИТИВНЫЕ ПАРАМЕТРЫ СМЕСИ (молекулярная масса и газовая постоянная)
  double mol_mass = 0.0,
         acent_ft = 1.0;

  for (auto const &x : components) {
    critical_pt_val[0] += x.first * x.second.first.V_K;
    critical_pt_val[1] += x.first * x.second.first.P_K;
    critical_pt_val[2] += x.first * x.second.first.T_K;

    mol_mass += x.first * x.second.first.molecularmass;
    acent_ft *= x.first * x.second.first.acentricfactor;
  }
  acent_ft = std::pow(acent_ft, 1.0 / components.size());
  // инициализируем постоянные параметры
  // init gasmix const_parameters
  std::unique_ptr<const_parameters> tmp_cgp(const_parameters::Init(
      critical_pt_val[0], critical_pt_val[1], critical_pt_val[2],
      /* 0.0 throw excep */ acent_ft, mol_mass));
  if (tmp_cgp == nullptr) {
    set_error_code(ERR_INIT | ERR_GAS_MIX | ERR_CALC_GAS_P);
    // set_error_message
    return nullptr;
  }
  // инициализируем динамические параметры
  //   т.е. перерасчитаем их для переданных параметров prs
  // init gasmix dyn_parameters
  // создадим временную копию динамических параметров
  //   которую потом пересчитаем к prs
  std::vector<std::pair<double, dyn_parameters>> dgp_cpt;
  for (auto const &x : components) {
    dgp_cpt.push_back({x.first, x.second.second});
    update_f(dgp_cpt.back().second, prs);
  }
  std::array<double, 2> dgp_tmp = {0.0, 0.0};// 0.0, 0.0};
  for (auto const &x : dgp_cpt) {
    dgp_tmp[0] += x.first * x.second.heat_cap_vol;
    dgp_tmp[1] += x.first * x.second.heat_cap_pres;
    // next two is some shit
   // dgp_tmp[2] += components[i].first * dgp_cpt[i].interval_energy;
  //  dgp_tmp[3] += components[i].first * dgp_cpt[i].beta_kr;
  }
  std::unique_ptr<dyn_parameters> tmp_dgp(dyn_parameters::Init(
      dgp_tmp[0], dgp_tmp[1], prs));
  return new GasParameters_mix_dyn(prs, *tmp_cgp, *tmp_dgp,
      components, update_f);
}

 void GasParameters_mix_dyn::csetParameters(double v, double p, double t,
    state_phase sp) {
  std::swap(prev_vpte_, vpte_);
  vpte_.volume       = v;
  vpte_.pressure     = p;
  vpte_.temperature  = t;
  sph_ = sp;
  // std::vector<dyn_parameters> dgp_cpt(components_.size());
  for (auto &x : components_)
    update_f_(x.second.second, vpte_);
  dyn_parameters_.heat_cap_vol  = 0.0;
  dyn_parameters_.heat_cap_pres = 0.0;
  dyn_parameters_.interval_energy = 0.0;
  dyn_parameters_.beta_kr = 0.0;
  dyn_parameters_.parm = vpte_;
  for (auto const &x : components_) {
    dyn_parameters_.heat_cap_vol  += x.first * x.second.second.heat_cap_vol;
    dyn_parameters_.heat_cap_pres += x.first * x.second.second.heat_cap_pres;
    dyn_parameters_.interval_energy +=
        x.first * x.second.second.interval_energy;
    dyn_parameters_.beta_kr += x.first * x.second.second.beta_kr;
  }
}
