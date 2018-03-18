#ifndef _CORE__GAS_PARAMETERS__GAS_DESCRIPTION_DYNAMIC_H_
#define _CORE__GAS_PARAMETERS__GAS_DESCRIPTION_DYNAMIC_H_

#include "gas_description.h"
#include "gas_description_static.h"

#include <iostream>

/*
 * PARAMETERS OF GAS IN DYNAMIC.
 *
 * Параметры газа в динамике.
 *   Основная структура данных
 * По сути это совокупность других структур
 *   с элементами ООП
*/
/// DEVELOP
///   Классификатор final классная штука:
///     -- не даёт наследываться
///     -- компилятор лучше обрабатывает перегруженные функции

class GasParameters_dyn final: public GasParameters {
public:
  GasParameters_dyn(double v, double p, double t,
      const_parameters cgp, dyn_parameters dgp,  dyn_params_update update_f);
  GasParameters_dyn(parameters prs,
      const_parameters cgp, dyn_parameters dgp, dyn_params_update update_f);
  GasParameters_dyn(const_parameters cgp, dyn_parameters dgp,
      dyn_params_update update_f);

  void csetParameters(double v,
      double p, double t, state_phase) override;

private:
  // previous pressure, volume and temperature
  parameters        prev_vpte_;
  // function for update dyn_parameters
  dyn_params_update update_f_;
  // potentials      potentials_;
};

std::ostream& operator<< (std::ostream &outstream,
    const GasParameters_dyn &gp);

#endif  // _CORE__GAS_PARAMETERS__GAS_DESCRIPTION_DYNAMIC_H_
