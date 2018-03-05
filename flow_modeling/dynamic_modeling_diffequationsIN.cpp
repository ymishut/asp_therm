#include "dynamic_modeling_diffequationsIN.h"
#include "models_exceptions.h"
#include "model_ideal_gas.h"
#include "model_peng_robinson.h"
#include "model_redlich_kwong.h"

//================================
// INTOdXdtPR ctor
//================================

real_gas_models::INTOdXdtPR::INTOdXdtPR(Peng_Robinson &mg, const balloon &bl,
                                                          gasparameters &outbl)
  :dXdt(bl, outbl), mg_(mg), a_(mg.getCoefA()), b_(mg.getCoefB()),
                                                 k_(mg.getCoefK()) {
  const float      alf = std::pow((1.0f + k_* (1.0f -
            std::sqrt(outbl_.cgetTemperature() / outbl_.cgetT_K()))), 2.0f),
              derivalf = -std::sqrt(alf) * 0.5f * k_ /
                std::sqrt(outbl_.cgetTemperature() * outbl_.cgetT_K());
                 hout_ = outbl_.cgetCV() * outbl_.cgetTemperature() -
            a_ * (alf - derivalf * outbl_.cgetTemperature()) *
            std::log(std::abs((2.414f*b_ + outbl_.cgetVolume()) / 
            (0.414f*b_ - outbl_.cgetVolume())))/ (2.828f*b_) +
      outbl_.cgetPressure()*outbl_.cgetVolume();
}

//================================
// INTOdXdtPR operator()
//================================

void real_gas_models::INTOdXdtPR::operator() (const difresult_t &x,
                                          difresult_t &dxdt, float) {
  float  G,
         w;
  calculateGW(G, w, mg_.getPressure(1.0 / x[0], x[1]), outbl_.cgetPressure(),
                                                       outbl_.cgetVolume());
  const float alf      = std::pow((1.0f + k_* (1.0f - std::sqrt(x[1] / 
            mg_.getT_K()))), 2.0f),
              derivalf = -std::sqrt(alf) * 0.5f * k_ / 
            std::sqrt(x[1]*mg_.getT_K()),
              houtbl   = hout_ + w * w / 2.0f,
              u        = mg_.getCV() * x[1] - a_ * (alf - derivalf*x[1]) *
            std::log(std::abs((2.414f*b_ + 1.0f/x[0])/(0.414f*b_-1.0f/x[0])))/
            (2.828f * b_);
  dxdt[0] = G / bl_.capacity;
  dxdt[1] = (G * (houtbl - u + bl_.capacity * x[0] * a_ * alf *
        ((0.414f*b_ - 1.0f/x[0])*2.828f * b_)) / (2.828f*b_*x[0]*x[0] * 
        bl_.capacity* std::pow(2.414f * b_ + (1.0f/x[0]), 3.0f)))
      / (bl_.capacity*x[0]*outbl_.cgetCV());
}

//================================
// INTOdXdtRK2 ctor
//================================

real_gas_models::INTOdXdtRK2::INTOdXdtRK2(Redlich_Kwong2 &mg, 
                       const balloon &bl, gasparameters &outbl)
  :dXdt(bl, outbl), mg_(mg), a_(mg.getCoefA()), b_(mg.getCoefB()) {
  hout_ = outbl_.cgetCV() * outbl_.cgetTemperature() -1.5f * a_ *
    std::log(1.0f+b_/outbl_.cgetVolume()) / 
    (b_*std::sqrt(outbl_.cgetTemperature())) +
    outbl_.cgetPressure()*outbl_.cgetVolume();
}

//================================
// INTOdXdtRK2 operator ()
//================================

void real_gas_models::INTOdXdtRK2::operator() (const difresult_t &x, 
                                           difresult_t &dxdt, float) {
  float  G,
         w;
  calculateGW(G, w, mg_.getPressure(1.0/x[0], x[1]), outbl_.cgetPressure(), 
                                                     outbl_.cgetVolume());
  const float houtbl = hout_ + w * w / 2.0f,
              u      = mg_.getCV() * x[1] -1.5f * a_ * 
            std::log(1.0f + b_*x[0]) / b_ / std::sqrt(x[1]);
  dxdt[0] = G / bl_.capacity;
  dxdt[1] = G * (houtbl - u + 1.5f * a_ * x[0] / (1.0f + b_*x[0]) /
      std::sqrt(x[1])) / (bl_.capacity * x[0] * (mg_.getCV() + 3.0f * a_ *
      std::log(1.0f + b_*x[0]) / (4.0f * b_ * std::pow(x[1], 1.5f))));
}

//================================
// INTOdXdtIdGas ctor
//================================

real_gas_models::INTOdXdtIdGas::INTOdXdtIdGas(idealGas &mg,
                     const balloon &bl, gasparameters &outbl)
  :dXdt(bl, outbl), mg_(mg) {}

//================================
// INTOdXdtIdGas operator()
//================================

void real_gas_models::INTOdXdtIdGas::operator() (const difresult_t &x,
                                             difresult_t &dxdt, float) {
  float  G,
         w;
  calculateGW(G, w, mg_.getPressure(1.0 / x[0], x[1]), outbl_.cgetPressure(),
                                                        outbl_.cgetVolume());
  dxdt[0] = G / bl_.capacity;
  dxdt[1] = G * (outbl_.cgetAdiabatic() * outbl_.cgetTemperature() - x[1]) /
      (bl_.capacity * x[0]);
}

