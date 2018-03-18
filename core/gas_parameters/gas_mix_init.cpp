#include "gas_mix_init.h"

#include "models_errors.h"

#include <assert.h>

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
