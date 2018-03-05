#include "model_general.h"
#include "gas_description_dynamic.h"
#include "gas_description_static.h"
#include "models_exceptions.h"

//================================
// modelGeneral ctor
//================================

real_gas_models::modelGeneral::modelGeneral(modelName mn,
                  std::shared_ptr<constgasparameters> &cgp)
  :parameters_(std::unique_ptr<Igasparameters> (
        new gasparameters(0.0, 0.0, 0.0, cgp))),
    phasediag_model_(mn),
    bp_(phasediagram::getCalculated().getBinodalPoints(cgp->V_K, cgp->P_K,
                         cgp->T_K, phasediag_model_, cgp->acentricfactor)) {
}

//================================
// modelGeneral::setParameters
//================================

void real_gas_models::modelGeneral::setParameters(float v, float p, float t) {
  parameters_->csetParameters(v, p, t, setState_phase(v, p, t));
}

//================================
// modelGeneral::setState_phasesub
//================================

int real_gas_models::modelGeneral::setState_phasesub(float p) {
  if (bp_.p.empty())
    throw modelExceptions("real_gas_models::modelGeneral::setState_phasesub");
  return (bp_.p.end() - std::find_if(bp_.p.begin() + 1, bp_.p.end(),
        std::bind2nd(std::less_equal<float>(), p)));
}

//================================
// modelGeneral::setState_phase
//================================

real_gas_models::state_phase real_gas_models::modelGeneral::setState_phase(
                                                 float v, float p, float t) {
  if (t >= parameters_->cgetT_K())
    return (p >= parameters_->cgetP_K()) ? state_phase::SCF : state_phase::GAS;

  // if p on the left of binodal graph  -  liquid
  int iter = setState_phasesub(p);
  if (!iter) {
      std::cout << " modelGeneral: gas have too low pressure\n";
      return ((v <= parameters_->cgetV_K()) ?
          state_phase::LIQ_STEAM : state_phase::GAS);
    }
  iter = bp_.p.size() - iter - 1;
  const float p_path = (p-bp_.p[iter+1]) / (bp_.p[iter]-bp_.p[iter+1]);   // %
  // left branch of binodal
  if (v < parameters_->cgetV_K()) {                 
      const float vapprox = bp_.vLeft[iter] - (bp_.vLeft[iter+1] -
          bp_.vLeft[iter]) * p_path;
      return ((v < vapprox) ? state_phase::LIQUID : state_phase::LIQ_STEAM);
    }
  const float vapprox = bp_.vRigth[iter] + (bp_.vRigth[iter+1] -
        bp_.vRigth[iter])*p_path;
  return ((v > vapprox) ? state_phase::GAS : state_phase::LIQ_STEAM);
}

//================================
// modelGeneral    getters
//================================

float real_gas_models::modelGeneral::getVolume() const {
  return parameters_->cgetVolume();
}

float real_gas_models::modelGeneral::getPressure() const {
  return parameters_->cgetPressure();
}

float real_gas_models::modelGeneral::getTemperature() const {
  return parameters_->cgetTemperature();
}

float real_gas_models::modelGeneral::getCV() const {
  return parameters_->cgetCV();
}

float real_gas_models::modelGeneral::getAcentric() const {
  return parameters_->cgetAcentricFactor();
}

float real_gas_models::modelGeneral::getAdiabatic() const {
  return parameters_->cgetAdiabatic();
}

float real_gas_models::modelGeneral::getT_K() const {
  return parameters_->cgetT_K();
}

real_gas_models::state_phase real_gas_models::modelGeneral::getState() const {
  return parameters_->cgetState();
}

real_gas_models::parameters real_gas_models::modelGeneral::getParametersCopy() const {
  return parameters_->cgetParameters();
}

std::shared_ptr<real_gas_models::constgasparameters> real_gas_models::modelGeneral::getConstParameters() const {
  return parameters_->cgetConstparameters();
}

//================================
// modelGeneral::setDynamicParameters
//================================

void real_gas_models::modelGeneral::setDynamicParameters() {
  std::unique_ptr<Igasparameters> temp(new gasparameters_dynamic(getParametersCopy(), getConstParameters()));
  temp.swap(parameters_);
}

//================================
// modelGeneral::setStaticParameters
//================================

void real_gas_models::modelGeneral::setStaticParameters() {
  Igasparameters *ptr;
  try {
    ptr = new gasparameters(getParametersCopy(), getConstParameters());
  } catch(std::exception &) {
    ptr = nullptr;
  }
  std::unique_ptr<Igasparameters> temp(ptr);
  temp.swap(parameters_);
}

//================================
// gasparametersConverter ctor
//================================

real_gas_models::gasparametersConverter::gasparametersConverter(modelGeneral *md)
  :md_(md) {
  md_->setDynamicParameters();
}

//================================
// gasparametersConverter dtor
//================================

real_gas_models::gasparametersConverter::~gasparametersConverter() {
  md_->setStaticParameters();
}
