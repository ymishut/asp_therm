#ifndef _CORE__MODELS__MODELS_CREATOR_H_
#define _CORE__MODELS__MODELS_CREATOR_H_

#include "model_general.h"
#include "model_ideal_gas.h"
#include "model_redlich_kwong.h"
#include "model_peng_robinson.h"


#include "subroutins/inputdata_by_file.h"

#include <memory>

class Equation_of_state {
public:
  virtual modelGeneral *GetCalculatingModel(modelName mn,
      const_parameters cgp, dyn_parameters dgp) = 0;
  virtual ~Equation_of_state();
};

class Ideal_gas_equation: public Equation_of_state {
public:
   modelGeneral *GetCalculatingModel(modelName mn,
       const_parameters cgp, dyn_parameters dgp);
};

class Redlich_Kwong_equation: public Equation_of_state {
public:
   modelGeneral *GetCalculatingModel(modelName mn,
       const_parameters cgp, dyn_parameters dgp);
};

class Peng_Robinson_equation: public Equation_of_state {
public:
   modelGeneral *GetCalculatingModel(modelName mn,
       const_parameters cgp, dyn_parameters dgp);
};

// DEVELOP to new file
/*
//=========================================================================
// Dynamic
//=========================================================================

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
*/
#endif  // _CORE__MODELS__MODELS_CREATOR_H_
