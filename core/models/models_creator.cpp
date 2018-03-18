#include "models_creator.h"

#include "models_errors.h"

#include "gas_parameters/gas_description.h"
#include "subroutins//models_output.h"
#include "subroutins/inputdata_by_file.h"
#include "phase_diagram/phase_diagram.h"
// #include "dynamic_modeling.h"

#include <boost/optional.hpp>

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <utility>


Equation_of_state::~Equation_of_state() {}


modelGeneral *Ideal_gas_equation::GetCalculatingModel(
    modelName mn = modelName::REDLICH_KWONG2,
    const_parameters cgp, dyn_parameters dgp) {
  // calculate binodal points
  reset_error();
  binodalpoints bp = PhaseDiagram::GetCalculated().GetBinodalPoints(
       cgp.V_K, cgp.P_K, cgp.T_K, phasediag_model_, cgp.acentricfactor);
  // check calculated bp
  if (bp.p.empty()) {
    std::cerr << get_error_message();
    std::cerr << "\n Could not create Idealgas Solver\n" << std::endl;
    return nullptr;
  }
  return IdealGas::Init(mn, cgp, dgp, bp);
}

modelGeneral *Redlich_Kwong_equation::GetCalculatingModel(
    modelName mn = modelName::REDLICH_KWONG2,
    const_parameters cgp, dyn_parameters dgp) {
  // calculate binodal points
  reset_error();
  binodalpoints bp = PhaseDiagram::GetCalculated().GetBinodalPoints(
       cgp.V_K, cgp.P_K, cgp.T_K, phasediag_model_, cgp.acentricfactor);
  // check calculated bp
  if (bp.p.empty()) {
    std::cerr << get_error_message();
    std::cerr << "\n Could not create RedlichKwong2 Solver\n" << std::endl;
    return nullptr;
  }
  return Redlich_Kwong2::Init(mn, cgp, dgp, bp);
}

modelGeneral *Peng_Robinson_equation::GetCalculatingModel(
    modelName mn = modelName::PENG_ROBINSON,
    const_parameters cgp, dyn_parameters dgp) {
  // calculate binodal points
  reset_error();
  binodalpoints bp = PhaseDiagram::GetCalculated().GetBinodalPoints(
       cgp.V_K, cgp.P_K, cgp.T_K, phasediag_model_, cgp.acentricfactor);
  // check calculated bp
  if (bp.p.empty()) {
    std::cerr << get_error_message();
    std::cerr << "\n Could not create PengRobinson Solver\n" << std::endl;
    return nullptr;
  }
  return Peng_Robinson::Init(mn, cgp, dgp, bp);
}

/*  to another file

// Конфигурация расчёта, может изменяться
//   если файл кфг изменился, или ещё что
// Определены в subroutins/load_config.cpp
extern const double MODELS_DTIME;
extern const size_t MODELS_STEPS;

namespace {
  std::map<std::string, int32_t> equations {
      {"IDEAL_GAS", 1}, {"REDLICH_KWONG", 2}, {"PENG_ROBINSON", 3}};
}
//================================
// GasDynamic::getFlowCalculator
//================================

balloonFlowDynamic *
GasDynamic::getFlowCalculator(
                 const std::shared_ptr<modelGeneral> &spmg,
               gasparameters &outballoon, double V, double F) {
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

void GasDynamic::calculation(std::shared_ptr<InputData> &idp) {
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

balloonFlowDynamic *GasDynamic::setCalculator(
                             std::shared_ptr<modelGeneral> &mg,
                                            std::shared_ptr<gasparameters> &pgp) {
  std::vector<double> params       = idp_->getBalloonParameters();
  std::pair<double, double> balloon = idp_->getBalloonVF();
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

std::shared_ptr<Equation_of_state>
GasDynamic::setEOS() {
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
*/
