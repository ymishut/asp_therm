#include "gas_description.h"
#include "models_math.h"
#include "models_exceptions.h"

//=========================================================================
// dyn_parameters
//=========================================================================
dyn_parameters::dyn_parameters(double ai, double cv, parameters pm)
  : adiabatic_index(ai), heat_cap_vol(cv), beta_kr(0.0), parm(pm) {
  beta_kr = std::pow(2.0 / (ai + 1.0), ai / (ai - 1.0));
}

dyn_parameters *dyn_parameters::Init(double ai, double cv, parameters pm) {
  // проверка входных данных
  // check input data:
  //  --  больше нуля     --  > 0.0
  //  --  конечны         --  finite
  bool correct_input = (ai != 1.0);
  correct_input &= is_above0(ai, cv);
  correct_input &= is_above0(pm.pressure, pm.temperature, pm.volume);
  if (!correct_input)
    return nullptr;
  return new dyn_parameters(ai, cv, pm);
}

//=========================================================================
// const_parameters
//=========================================================================
const_parameters::const_parameters(double vk, double pk,
    double tk, double mol, double R, double af)
  : V_K(vk), P_K(pk), T_K(tk), molecularmass(mol),
    R(R), acentricfactor(af) {}

const_parameters
*const_parameters::Init(double vk, double pk, double tk,
    double mol, double af) {
  /// DEVELOP
  ///   Введение переменных 'correct_input' или
  ///     'tempR' может показаться излишним многословием,
  ///     но это облегчает поддержку кода
  bool correct_input = is_above0(vk, pk, tk, mol, af);
  if (!correct_input)
    return nullptr;
  // 8314.4599 - универсальная газовая постоянная
  //             GAS CONSTANT - physics constant
  double tempR = 8314.4599 / mol;
  return new const_parameters(vk, pk, tk, mol, tempR, af);
}

/*
const_parameters
*const_parameters::Init(std::array<double, 5> vec) {
  return Init(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6]);
}
*/
