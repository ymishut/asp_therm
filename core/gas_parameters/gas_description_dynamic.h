#ifndef SRC_GAS_DESCRIPTION_DYNAMIC_H_
#define SRC_GAS_DESCRIPTION_DYNAMIC_H_

#include <iostream>
#include <memory>

#include "gas_description.h"

namespace real_gas_models {
  class gasparameters_dynamic;
  //================================
  // potentials (class)
  //================================

  class potentials {
    friend class gasparameters_dynamic;
    friend std::ostream& operator<< (std::ostream & outstream, 
                                          const potentials &pt);

    float  internalenergy,
           Hermholtz_free,
           enthalpy,
           Gibbsfree,
           LandauGrand,
           // entropy not potential but calculating in dynamic have sense
           entropy;    
    potentials() {}
  };

  std::ostream& operator<< (std::ostream & outstream, const potentials &pt);

  //================================
  // gasparameters_dynamic
  //================================

  class gasparameters_dynamic: public Igasparameters {
  public:
    gasparameters_dynamic(float v, float p, float t,
             std::shared_ptr<constgasparameters> cgp);
    gasparameters_dynamic(parameters prs, 
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
    std::shared_ptr <constgasparameters> cgetConstparameters() const;

    void csetParameters(float v, float p, float t, state_phase sp);

  private:
    parameters            current_vpte_,
                          previous_vpte_;
    potentials            potentialslist_;
    state_phase           sph_;
    std::shared_ptr<constgasparameters> constparameters_;
  };

  std::ostream& operator<< (std::ostream &outstream,
                     const gasparameters_dynamic &gp);
}  // namespace real_gas_models

#endif  // SRC_GAS_DESCRIPTION_DYNAMIC_H_

