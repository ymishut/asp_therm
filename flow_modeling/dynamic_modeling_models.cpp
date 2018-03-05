#include "dynamic_modeling_models.h"
#include "model_ideal_gas.h"
#include "model_peng_robinson.h"
#include "model_redlich_kwong.h"
#include "dynamic_modeling_diffequationsIN.h"
#include "dynamic_modeling_diffequationsOUT.h"

//================================
// dXdtobserver ctor
//================================

real_gas_models::dXdtobserver::dXdtobserver(std::shared_ptr<modelGeneral> &pmg)
  :pmg_(pmg) {}

//================================
// dXdtobserver operator()
//================================

void real_gas_models::dXdtobserver::operator() (const difresult_t &x, float t) {
  (void) t;
  pmg_->setPressure(1.0f / x[0], x[1]);
}

//================================
// DerivateFunctorInflow ctor
//================================

real_gas_models::DerivateFunctorInflow::DerivateFunctorInflow(const balloon &bl,
                                        gasparameters &outbl, modelGeneral *pmg)
  :bl_(bl), outbl_(outbl) {
  pmg->dynamicflowAccept(*this);
}

//================================
// DerivateFunctorInflow getFunctor (...)
//================================

void real_gas_models::DerivateFunctorInflow::getFunctor(idealGas &mg) {
  derivfunc_ = INTOdXdtIdGas(mg, bl_, outbl_);
}

void real_gas_models::DerivateFunctorInflow::getFunctor(Redlich_Kwong2 &mg) {
  derivfunc_ = INTOdXdtRK2(mg, bl_, outbl_);
}

void real_gas_models::DerivateFunctorInflow::getFunctor(Peng_Robinson &mg) {
  derivfunc_ = INTOdXdtPR(mg, bl_, outbl_);
}

//================================
// DerivateFunctorOutflow ctor
//================================

real_gas_models::DerivateFunctorOutflow::DerivateFunctorOutflow(
                                const real_gas_models::balloon &bl,
                             real_gas_models::gasparameters &outbl,
                                real_gas_models::modelGeneral *pmg)
  :bl_(bl), outbl_(outbl) {
  pmg->dynamicflowAccept(*this);
}

//================================
// DerivateFunctorOutflow getFunctor (...)
//================================

void real_gas_models::DerivateFunctorOutflow::getFunctor(
                           real_gas_models::idealGas &mg) {
  derivfunc_ = OUTdXdtIdGas(mg, bl_, outbl_);
}

void real_gas_models::DerivateFunctorOutflow::getFunctor(
                     real_gas_models::Redlich_Kwong2 &mg) {
  derivfunc_ = OUTdXdtRK2(mg, bl_, outbl_);
}

void real_gas_models::DerivateFunctorOutflow::getFunctor(
                      real_gas_models::Peng_Robinson &mg) {
  derivfunc_ = OUTdXdtPR(mg, bl_, outbl_);
}
