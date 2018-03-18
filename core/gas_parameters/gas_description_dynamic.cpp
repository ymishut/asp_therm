#include "gas_description_dynamic.h"

#include <cmath>
#include <utility>

#include <assert.h>

GasParameters_dyn::GasParameters_dyn(double v, double p, double t,
    const_parameters cgp, dyn_parameters dgp,
    dyn_params_update update_f)
  : GasParameters(v, p, t, cgp, dgp), prev_vpte_({v, p, t}),
    update_f_(update_f) {}

GasParameters_dyn::GasParameters_dyn(parameters prs,
    const_parameters cgp, dyn_parameters dgp,
    dyn_params_update update_f)
  : GasParameters(prs, cgp, dgp), prev_vpte_(prs),
    update_f_(update_f) {}

GasParameters_dyn::GasParameters_dyn(
    const_parameters cgp, dyn_parameters dgp,
    dyn_params_update update_f)
  : GasParameters(dgp.parm, cgp, dgp),
    prev_vpte_(dgp.parm), update_f_(update_f) {}


void GasParameters_dyn::csetParameters(double v, double p,
    double t, state_phase sp) {
  std::swap(prev_vpte_, vpte_);
  vpte_.volume       = v;
  vpte_.pressure     = p;
  vpte_.temperature  = t;
  sph_               = sp;
  update_f_(dyn_parameters_, vpte_);
 // assert(0);
  // update dyn_pars
  // updatePotentials();
}

std::ostream &operator<< (std::ostream &outstream,
    const GasParameters_dyn &gp) {
  outstream << "v: " << gp.cgetVolume() << " p: " << gp.cgetPressure()
            << " t: " << gp.cgetTemperature() << "\n";
  return outstream;
}
