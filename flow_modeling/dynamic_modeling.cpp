#include "dynamic_modeling.h"
#include "gas_description_static.h"
#include "dynamic_modeling_calculate.h"
#include "dynamic_modeling_diffequationsIN.h"
#include "dynamic_modeling_diffequationsOUT.h"
#include "dynamic_modeling_models.h"
#include "models_exceptions.h"

extern float MODELS_CALC_ACCUR;
float real_gas_models::balloonFlowDynamic::accuracy_;

//================================
// balloonFlowDynamic ctor
//================================

real_gas_models::balloonFlowDynamic::balloonFlowDynamic(
                            const std::shared_ptr<modelGeneral> &spmg,
                                            gasparameters &outballoon,  
                                          const float V, const float F)
  :calculateModel_(spmg),
    calculateModelConverter_(spmg.get()),
    outbl_(outballoon),
    bl_(V, F) {
  if ((V <= 0.0) || (F <= 0.0) || 
      (!std::isfinite(F)) || (!std::isfinite(V))) {
      throw modelExceptions(
          "balloonFlowDynamic constructor get not correct args V or F");
    }
  accuracy_ = MODELS_CALC_ACCUR;                                        
  parameters rmsM  = calculateModel_->getParametersCopy(),
             rmsOB = outbl_.cgetParameters();
  // not common case
  if (rmsOB.pressure == 0.0) {                                                             
      calculateModel_->setPressure(rmsOB.volume, rmsOB.temperature);                               /// calculate all outbl_ parameters
      outbl_.csetParameters(calculateModel_->getVolume(), 
          calculateModel_->getPressure(), calculateModel_->getTemperature(),
          calculateModel_->getState());
      calculateModel_->setPressure(rmsM.volume, rmsM.temperature);                                 /// return in model previous parameters
    } else {
      try {
        calculateModel_->setVolume(rmsOB.pressure, rmsOB.temperature);
        outbl_.csetParameters(calculateModel_->getVolume(),
            calculateModel_->getPressure(), calculateModel_->getTemperature(),
            calculateModel_->getState());
        calculateModel_->setPressure(rmsM.volume, rmsM.temperature);
        return;
      } catch(modelExceptions &e) {
        std::cout << e.what() << std::endl;
      }
      throw modelExceptions(
          "balloonFlowDynamic constructor get not correct args in outballoon.");
    }
}

//================================
// balloonFlowDynamic calculateFlow <>
//================================

template
void real_gas_models::balloonFlowDynamic::calculateFlow <
    class real_gas_models::DerivateFunctorInflow>(const float dtime,
                       const size_t outRate, formatted_output &out);

template
void real_gas_models::balloonFlowDynamic::calculateFlow <
    class real_gas_models::DerivateFunctorOutflow>(const float dtime,
                       const size_t outRate, formatted_output &out);

template <class DerivateFunctorType>
void real_gas_models::balloonFlowDynamic::calculateFlow(
    const float dtime, const size_t outRate, formatted_output &out) {
  out << "\n";
  out << "time       |pressure   |density    |temperature\n";
  size_t step   = 0;
  float pmore   = outbl_.cgetPressure(),
        pless   = calculateModel_->getPressure(),
        time    = 0.0;
  setStartVariables<DerivateFunctorType>(pmore, pless, accuracy_);

  DerivateFunctorType dfi(bl_, outbl_, calculateModel_.get());
  auto fdxdtOBS = dXdtobserver(calculateModel_);
  auto fdxdt    = dfi.derivfunc_;
  stepper_t method;
  difresult_t x;
  while (pless < pmore) {
      if (!(step % outRate)) {
          out << time << calculateModel_->getPressure()
              << 1.0 / calculateModel_->getVolume()
              << calculateModel_->getTemperature() 
              << stateToString[(size_t)calculateModel_->getState()] << "\n";
        }
      try {
        x[0] = 1.0f / calculateModel_->getVolume();
        x[1] = calculateModel_->getTemperature();
      } catch (modelExceptions &e) {
        std::cout << e.what() << std::endl;
        break;
      }

      boost::numeric::odeint::integrate_n_steps(method, fdxdt, x,
                                       time, dtime, 1, fdxdtOBS);
      if (!calculateModel_->isValid()) {
          std::cout << "Now the results are not correct\n";
          break;
        }
      ++step;
      time += dtime;
      updatePress(pmore, pless, dfi);
    }
  out << time << calculateModel_->getPressure()
      << 1.0 / calculateModel_->getVolume()
      << calculateModel_->getTemperature()
      << stateToString[(size_t)calculateModel_->getState()] << "\n";
}

//================================
// balloonFlowDynamic updatePress
//================================

void real_gas_models::balloonFlowDynamic::updatePress(float &pm, float &pl,
                                   real_gas_models::DerivateFunctorInflow ) {
  (void)pm;
  pl = calculateModel_->getPressure();
}

void real_gas_models::balloonFlowDynamic::updatePress(float &pm, float &pl,
                                  real_gas_models::DerivateFunctorOutflow ) {
  (void)pl;
  pm = calculateModel_->getPressure();
}


