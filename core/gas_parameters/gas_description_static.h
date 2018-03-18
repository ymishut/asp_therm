#ifndef _CORE__GAS_PARAMETERS__GAS_DESCRIPTION_STATIC_H_
#define _CORE__GAS_PARAMETERS__GAS_DESCRIPTION_STATIC_H_

#include "gas_description.h"
#include "gas_mix_init.h"

#include <iostream>

/*
 * PARAMETERS OF GAS IN STATIC.
 *   Base class for dynamic gas parameters
 * Параметры газа в статике.
 *   Базовый класс для динамических параметров
 * По сути это совокупность других структур
 *   с элементами ООП
*/
class GasParameters {
protected:
  state_phase sph_;
  parameters  vpte_;
  const_parameters const_parameters_;
  // dyn_parameters содержат несколько важных полей
  dyn_parameters  dyn_parameters_;

public:
  GasParameters(double v, double p, double t,
      const const_parameters cgp, dyn_parameters dgp);
  GasParameters(parameters prs,
      const const_parameters cgp, dyn_parameters dgp);
  // for gas mix
  GasParameters(parameters prs, parameters_mix components);

  double cgetV_K()            const;
  double cgetP_K()            const;
  double cgetT_K()            const;
  double cgetMolecularMass()  const;
  double cgetR()              const;
  double cgetAcentricFactor() const;
  double cgetVolume()         const;
  double cgetPressure()       const;
  double cgetTemperature()    const;
  state_phase cgetState()     const;
  parameters cgetParameters() const;
  dyn_parameters cgetDynParameters() const;
  const_parameters cgetConstparameters() const;

  double cgetAdiabatic()  const;
  double cgetCV()         const;
  double cgetBeta()       const;

  virtual void csetParameters(double v, double p, double t, state_phase);

  virtual ~GasParameters();

private:
};

std::ostream& operator<< (std::ostream &outstream, const GasParameters &gp);

#endif  // _CORE__GAS_PARAMETERS__GAS_DESCRIPTION_STATIC_H_
