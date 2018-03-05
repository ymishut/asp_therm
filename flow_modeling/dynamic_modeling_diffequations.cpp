#include "dynamic_modeling_diffequations.h"
#include "models_exceptions.h"

//================================
// dXdt::calculateGW
//================================

void real_gas_models::dXdt::calculateGW(float &G, float &w, 
                                        const float p_less,
                                        const float p_more,
                                        const float vol_const) {
  // "pressure in balloon" / "flowinto pressure"
  const float p_PK = (p_less / p_more),         
              ai = outbl_.cgetAdiabatic();
  if (p_PK < outbl_.cgetBeta()) {
      G = bl_.nozzleSq * std::sqrt(ai * p_more *
          std::pow(2.0f / (ai + 1.0f), (ai+1.0f) / (ai-1.0f)) / vol_const);
      w = std::sqrt(2.0f * p_more * ai * vol_const / (ai + 1.0f));
    } else {
      G = bl_.nozzleSq * std::pow(p_PK, 1.0 / ai) * 
        std::sqrt(2.0f * ai * p_more * (1.0f - 
        std::pow(p_PK, (ai - 1.0f) / ai)) / 
        ((vol_const) * (ai-1.0f)));
      w = std::sqrt(2.0f * p_more * ai * vol_const / (ai -1.0f) *
          (1.0f - std::pow(p_PK, (ai - 1.0f) / ai)));
    }
  if ((G <= 0.0) || (w <= 0.0) ||
      (!std::isfinite(G)) || (!std::isfinite(w)))
    throw modelExceptions(
        "dynamic_modeling_diffequations calculateGW bad results");
}

//================================
// dXdt ctor
//================================

real_gas_models::dXdt::dXdt(const balloon &bl, gasparameters &outbl)
  :bl_(bl), outbl_(outbl) {}

//================================
// balloon ctor
//================================

real_gas_models::balloon::balloon(const float V, const float F)
  :capacity(V), nozzleSq(F) {}
