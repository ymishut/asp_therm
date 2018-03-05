#include "dynamic_modeling_diffequationsOUT.h"
#include "models_exceptions.h"
#include "model_ideal_gas.h"
#include "model_peng_robinson.h"
#include "model_redlich_kwong.h"

//================================
// OUTdXdtPR ctor
//================================

real_gas_models::OUTdXdtPR::OUTdXdtPR(Peng_Robinson &mg, const balloon &bl,
                                                        gasparameters &outbl)
  :dXdt(bl, outbl), mg_(mg), a_(mg.getCoefA()), b_(mg.getCoefB()),
                                                 k_(mg.getCoefK()) {}

//================================
// OUTdXdtPR operator()
//================================

void real_gas_models::OUTdXdtPR::operator() (const difresult_t &x, 
                                         difresult_t &dxdt, float) {
  float  G,
         w;
  calculateGW(G, w, outbl_.cgetPressure(),
    mg_.getPressure(1.0 / x[0], x[1]), 1.0 / x[0]);
  const float alf      = std::pow((1.0f + k_* (1.0f - 
            std::sqrt(x[1] / mg_.getT_K()))), 2.0f),
              derivalf = -std::sqrt(alf) * 0.5f * k_ /
            std::sqrt(x[1]*mg_.getT_K()),
              houtbl   = mg_.getCV() * x[1] - a_*(alf-derivalf*x[1]) *
            std::log(std::abs((2.414f*b_ + 1.0/x[0])/(0.414f*b_ -1.0/x[0]))) /
            (2.828f*b_) + mg_.getPressure(1.0/x[0], x[1])*1.0/x[0] + w*w /2.0f,
              u        = mg_.getCV()*x[1] - a_*(alf - derivalf*x[1]) *
            std::log(std::abs((2.414f*b_ + 1.0f/x[0]) /
            (0.414f*b_ - 1.0f/x[0])) ) / (2.828f * b_);
  dxdt[0] = - G/bl_.capacity;
  dxdt[1] = G * (-houtbl - u + bl_.capacity * x[0] * a_ * alf *
      ((0.414f*b_ - 1.0f/x[0])*2.828f * b_) / (2.828f*b_*x[0]*x[0] *
      bl_.capacity* std::pow(2.414f * b_ + (1.0f/x[0]), 3.0f)))
      / (bl_.capacity*x[0]*mg_.getCV());
}

//================================
// OUTdXdtRK2 ctor
//================================

real_gas_models::OUTdXdtRK2::OUTdXdtRK2(Redlich_Kwong2 &mg, const balloon &bl,
                                                           gasparameters &outbl)
  :dXdt(bl, outbl), mg_(mg), a_(mg.getCoefA()), b_(mg.getCoefB()) {}

//================================
// OUTdXdtRK operator()
//================================

void real_gas_models::OUTdXdtRK2::operator() (const difresult_t &x,
                                          difresult_t &dxdt, float) {
  float  G,
         w;
  calculateGW(G, w, outbl_.cgetPressure(),
      mg_.getPressure(1.0/x[0], x[1]), 1.0/x[0]);
  const float houtbl = mg_.getCV()*x[1] -1.5f*a_*std::log(1.0f+b_*x[0]) /
            (b_*std::sqrt(x[1]))+mg_.getPressure(1.0/x[0], x[1])/x[0]+w*w/2.0f,
              u      = mg_.getCV()*x[1] -1.5f*a_*std::log(1.0f+b_*x[0]) /
            b_ / std::sqrt(x[1]);
  dxdt[0] = -G / bl_.capacity;
  dxdt[1] = G * (-houtbl-u+1.5f*a_*x[0]/(1.0f+b_*x[0])/std::sqrt(x[1])) /
      (bl_.capacity*x[0]*(mg_.getCV()+3.0f*a_*std::log(1.0f+b_*x[0]) /
      (4.0f*b_*std::pow(x[1], 1.5f))));
}

//================================
// OUTdXdtIdGas ctor
//================================

real_gas_models::OUTdXdtIdGas::OUTdXdtIdGas(idealGas &mg, const balloon &bl,
                                                         gasparameters &outbl)
  :dXdt(bl, outbl), mg_(mg) {}

//================================
// OUTdXdtIdGas operator()
//================================

void real_gas_models::OUTdXdtIdGas::operator() (const difresult_t &x,
                                            difresult_t &dxdt, float) {
  float  G,
         w;
  calculateGW(G, w, outbl_.cgetPressure(),
      mg_.getPressure(1.0/x[0], x[1]), 1.0/x[0]);
  dxdt[0] = -G / bl_.capacity;
  dxdt[1] = -G * (mg_.getAdiabatic() * mg_.getTemperature() - x[1]) /
      (bl_.capacity*x[0]);
}
