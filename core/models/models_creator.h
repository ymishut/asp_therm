#ifndef SRC_MODELS_CREATOR_H_
#define SRC_MODELS_CREATOR_H_

#include <memory>

#include "phase_diagram.h"

namespace real_gas_models {
  class modelGeneral;
  class InputData;
  class balloonFlowDynamic;
  class gasparameters;
  struct constgasparameters;

  //================================
  // Equation_of_state
  //================================

  class Equation_of_state {
  public:
    virtual modelGeneral *getCalculatingModel(modelName mn,
                          std::shared_ptr<constgasparameters> &cgp) = 0;
  };

  //================================
  // Ideal_gas_equation
  //================================

  class Ideal_gas_equation: public Equation_of_state {
  protected:
     modelGeneral *getCalculatingModel(modelName mn,
                          std::shared_ptr<constgasparameters> &cgp);
  };

  //================================
  // Redlich_Kwong_equation
  //================================

  class Redlich_Kwong_equation: public Equation_of_state {
  protected:
     modelGeneral *getCalculatingModel(modelName mn,
                         std::shared_ptr<constgasparameters> &cgp);
  };

  //================================
  // Peng_Robinson_equation
  //================================

  class Peng_Robinson_equation: public Equation_of_state {
  protected:
     modelGeneral *getCalculatingModel(modelName mn,
                         std::shared_ptr<constgasparameters> &cgp);
  };

  //================================
  // GasDynamic
  //================================

  class GasDynamic {
  public:
    balloonFlowDynamic *getFlowCalculator(
        const std::shared_ptr<modelGeneral> &spmg, gasparameters &outballoon,
                                                 const float V, const float F);

    void calculation(std::shared_ptr<InputData> &idp);

  private:
    std::shared_ptr<InputData> idp_;

  private:
    balloonFlowDynamic *setCalculator(std::shared_ptr<modelGeneral> &mg,
                                     std::shared_ptr<gasparameters> &pgp);

    std::shared_ptr<Equation_of_state> setEOS();
  };
}  // namespace real_gas_models

#endif  // SRC_MODELS_CREATOR_H_

