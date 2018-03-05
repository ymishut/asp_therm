#ifndef SRC_GAS_DESCRIPTION_STATIC_H_
#define SRC_GAS_DESCRIPTION_STATIC_H_

#include <iostream>
#include <memory>

#include "gas_description.h"

namespace real_gas_models {
class gasparameters:public Igasparameters {
public:
  gasparameters(float v, float p, float t,
        std::shared_ptr<constgasparameters> cgp);
  gasparameters(parameters prs,
        std::shared_ptr<constgasparameters> cgp);

  float cgetV_K()            const;
  float cgetP_K()            const;
  float cgetT_K()            const;
  float cgetMolecularMass()  const;
  float cgetAdiabatic()      const;
  float cgetCV()             const;
  float cgetBeta()           const;
  float cgetR()              const;
  float cgetAcentricFactor() const;
  float cgetVolume()         const;
  float cgetPressure()       const;
  float cgetTemperature()    const;
  state_phase cgetState()    const;
  parameters cgetParameters()const;
  std::shared_ptr<constgasparameters> cgetConstparameters() const;

  void csetParameters(float v, float p, float t, state_phase);

private:
  parameters vpte_;
  state_phase sph_;
  std::shared_ptr<constgasparameters> constparameters_;
};

std::ostream& operator<< (std::ostream &outstream, const gasparameters &gp);

}  // namespace real_gas_models

#endif  // SRC_GAS_DESCRIPTION_STATIC_H_

