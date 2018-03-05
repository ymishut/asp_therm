#include "models_creator.h"
#include "models_exceptions.h"
#include "models_output.h"
#include "dynamic_modeling.h"
#include "model_ideal_gas.h"
#include "model_redlich_kwong.h"
#include "model_peng_robinson.h"
#include "inputdata_by_file.h"
#include "gas_description.h"

#include <map>
#include <utility>
#include <functional>
#include <string>
#include <vector>

#include <boost/optional.hpp>

extern const float  MODELS_DTIME;
extern const size_t MODELS_STEPS;

namespace {
  std::map<std::string, int> equations {
      {"IDEAL_GAS", 1}, {"REDLICH_KWONG", 2}, {"PENG_ROBINSON", 3}};
}

//================================
// Ideal_gas_equation::getCalculatingModel
//================================

real_gas_models::modelGeneral *
real_gas_models::Ideal_gas_equation::getCalculatingModel(
    modelName mn, std::shared_ptr<constgasparameters> &cgp) {
  try {
    return new idealGas(mn, cgp);
  } catch (modelExceptions& e) {
    std::cout << "Object idealGas wasn't created.\n" 
              << e.what() << std::endl;
  }
  return nullptr;
}

//================================
// Redlich_Kwong_equation::getCalculatingModel
//================================

real_gas_models::modelGeneral *
real_gas_models::Redlich_Kwong_equation::getCalculatingModel(
    modelName mn, std::shared_ptr<constgasparameters> &cgp) {
  try {
    return new Redlich_Kwong2(mn, cgp);
  } catch (modelExceptions& e) {
    std::cout << "Object Redlich_Kwong2 wasn't created.\n" 
              << e.what() << std::endl;
  }
  return nullptr;
}

//================================
// Peng_Robinson_equation::getCalculatingModel
//================================

real_gas_models::modelGeneral *
real_gas_models::Peng_Robinson_equation::getCalculatingModel(
    modelName mn, std::shared_ptr<constgasparameters> &cgp) {
  try {
    return new Peng_Robinson(mn, cgp);
  } catch (modelExceptions& e) {
    std::cout << " Object Peng_Robinson wasn't created.\n" 
              << e.what() << std::endl;
  }
  return nullptr;
}

//================================
// GasDynamic::getFlowCalculator
//================================

real_gas_models::balloonFlowDynamic *
real_gas_models::GasDynamic::getFlowCalculator(
                 const std::shared_ptr<real_gas_models::modelGeneral> &spmg,
               real_gas_models::gasparameters &outballoon, float V, float F) {
  if (spmg == nullptr) {
      std::cout << 
        "Object FlowCalculator wasn't created. Constructor get nullptr\n";
      return nullptr;
    }
  try {
    return new balloonFlowDynamic (spmg, outballoon, V, F);
  } catch (modelExceptions &e) {
    std::cout << " Object FlowCalculator wasn't created.\n" 
              << e.what() << std::endl;
  }
  return nullptr;
}

//================================
// GasDynamic::calculation
//================================

void real_gas_models::GasDynamic::calculation(std::shared_ptr<InputData> &idp) {
  if (idp == nullptr) {
      std::cout << " Calculation method get nullptr to InputData";
      return;
    }
  idp_ = idp;
  std::shared_ptr<Equation_of_state> eos(setEOS());
  std::shared_ptr<constgasparameters> cgp(
      constgasparameters::getInstance(idp_->getConstgasparameters()));
  if (eos == nullptr) {
      std::cout << " Bad inputdata for Equation of state";
      return;
    }
  if (cgp == nullptr) {
      std::cout << " Bad inputdata for constgasparameters";
      return;
    }
  auto mg = std::shared_ptr<modelGeneral>(eos->getCalculatingModel(
        modelName::PENG_ROBINSON, cgp));
  if (mg == nullptr) {
      std::cout << " Bad inputdata for CalculatingModel";
      return;
    }
  std::shared_ptr<gasparameters> gp;
  std::unique_ptr<balloonFlowDynamic> up(setCalculator(mg, gp));
  if (up == nullptr) {
      std::cout << " Bad inputdata for setCalculator";
      return;
    }
  formatted_output SOout(std::cout);
  auto msgf = [] () { std::cout << " Bad inputdata for type of flow";};
  std::string InOutCheck = idp_->getFlowType();
  (InOutCheck == "IN") ? 
    up->calculateFlow<DerivateFunctorInflow>(MODELS_DTIME, MODELS_STEPS, SOout)
      :((InOutCheck == "OUT") ? 
         up->calculateFlow<DerivateFunctorOutflow>(MODELS_DTIME, MODELS_STEPS, SOout)
                          : msgf());
}

//================================
// GasDynamic::setCalculator
//================================

real_gas_models::balloonFlowDynamic *real_gas_models::GasDynamic::setCalculator(
                             std::shared_ptr<real_gas_models::modelGeneral> &mg,
                                            std::shared_ptr<gasparameters> &pgp) {
  std::vector<float> params       = idp_->getBalloonParameters();
  std::pair<float, float> balloon = idp_->getBalloonVF();
  char func = idp_->getFunction()[0];
  try {
    switch (func) {
      case 'V': {
          mg->setVolume(params[1], params[2]);
          pgp = std::make_shared<gasparameters> (0.0, params[4], params[5], 
                                                     mg->getConstParameters());
          return getFlowCalculator(mg, *pgp, balloon.first, balloon.second);
        }
      case 'P': {
          mg->setPressure(params[0], params[2]);
          pgp = std::make_shared <gasparameters> (params[3], 0.0, params[5],
                                                    mg->getConstParameters());
          return getFlowCalculator(mg, *pgp, balloon.first, balloon.second);
        }
      default:
        std::cout << " Bad inputdata in initfile FUNCTION";
        return nullptr;
      }
  }
  catch(modelExceptions &e) {
    std::cout << e.what() << std::endl;
    return nullptr;
  }
}

//================================
// GasDynamic::setEOS
//================================

std::shared_ptr<real_gas_models::Equation_of_state>
real_gas_models::GasDynamic::setEOS() {
  int eosch;
  std::shared_ptr<Equation_of_state> eos;
  try {
    eosch = equations.at(idp_->getEquation());
  } catch (std::exception &e) {
    std::cout << idp_->getEquation()
              << "\n Bad input cannot create Real_gas_equation creator!"
              << std::endl;
    return eos;
  }
  switch (eosch) {
    case 1:
      eos = std::shared_ptr<Equation_of_state> (new Ideal_gas_equation());
      break;
    case 2:
      eos = std::shared_ptr<Equation_of_state> (new Redlich_Kwong_equation());
      break;
    case 3:
      eos = std::shared_ptr<Equation_of_state> (new Peng_Robinson_equation());
      break;
    }
  return eos;
}
