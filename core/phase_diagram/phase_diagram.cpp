#include "phase_diagram.h"
#include "models_math.h"
#include "models_exceptions.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <utility>
#include <tuple>

//================================
// phasediagram::calculateBinodal
//================================

void real_gas_models::phasediagram::calculateBinodal(
         std::shared_ptr<binodalpoints>& bdp, modelName mn, double acentric) {
  const size_t nPoints = bdp->t.size();
  double    pi,
            dpi,            // current pressure and pressure correction
            splineAR,       // area under function t(p,v) from v0 to v2;
            rectanAR;       // area under pi=const, from v0 to v2;
                        // Maxwell construction requirement splineAR==rectanAR;

  auto integrateFun = lineIntegrate_.at(size_t(mn));
  auto inializeFun  = initialize_.at(size_t(mn));

  // get pressure by volume and temperature

  for (size_t t_iter = 0; t_iter < nPoints; ++t_iter) {
      std::vector<double> tempvec = { 1.0,
                                      -9.0/(4.0*bdp->t[t_iter]),
                                      6.0/(4.0*bdp->t[t_iter]),
                                      -1.0/(4.0*bdp->t[t_iter]),
                                      0.0, 0.0, 0.0};
    // calculate derivate(dpressure/dtemperature)=0
      try {
        if (CardanoMethod_HASUNIQROOT(&tempvec[0], &tempvec[4])) {
            bdp->t[t_iter] = -1.0;
            continue;
          }
      } catch (modelExceptions &e) {
        std::cout << e.what() << " for temperature index:"
                  << std::to_string(t_iter) << '\n';
        bdp->t[t_iter] = -1.0;
        continue;
      }
      std::sort(tempvec.begin() + 4, tempvec.end());

      assert((tempvec[4] >= 0.0) && (tempvec[5] >= 0.0) && (tempvec[6] >= 0.0));

      pi = bdp->t[t_iter]*bdp->t[t_iter]*bdp->t[t_iter];
         // another variant of start approximation
         //      pi = (8.0*bdp->t[t_iter]*tempvec[5]*tempvec[5] - 9.0*tempvec[5] +3.0)/
         //          (3.0*tempvec[5]*tempvec[5]*tempvec[5]-tempvec[5]*tempvec[5]);
      if (pi <= 0.0)
        pi = 0.01;
      dpi = -pi * 0.002;

      size_t trycount = 0;
      while (true) {
          if (trycount > 3000) {
              bdp->t[t_iter] = -1.0;
              break;
            }
          ++trycount;
          if (t_iter < 4)
            dpi *= 0.1;         // descrease dpi for field located nearby to critical point
          pi+=dpi;

          // calculate volume
          inializeFun(tempvec, pi, bdp->t[t_iter], acentric);

          try {
            if (CardanoMethod_HASUNIQROOT(&tempvec[0], &tempvec[4])) {
                dpi = 0.002*pi;
                pi  = (tempvec[4] <= 1.0) ? (pi - 2.0*dpi) : (pi + 2.0*dpi);
                continue;
              }
          } catch (modelExceptions &e) {
            std::cout << e.what();
            bdp->t[t_iter] = -1.0;
            break;
          }

          assert((tempvec[4] >= 0.0) && (tempvec[5] >= 0.0) && (tempvec[6] >= 0.0));

          std::sort(tempvec.begin() + 4, tempvec.end());
          if (tempvec[4] == tempvec[6]) {
              bdp->t[t_iter] = -1.0;
              break;
            }
          bdp->vLeft[t_iter]  = tempvec[4];
          bdp->vRigth[t_iter] = tempvec[6];
          rectanAR = (tempvec[6]-tempvec[4])*pi;

          splineAR = integrateFun(bdp->t[t_iter], bdp->vLeft[t_iter],
              bdp->vRigth[t_iter], acentric);

          double ARdiffer = (rectanAR -splineAR) / rectanAR;
          if (std::abs(ARdiffer) < 0.005) {
              bdp->p[t_iter] = pi;
              break;
            }
          else if (ARdiffer > 0.0)
              pi -= 2.0*dpi;
          else
              pi += 3.0*dpi;               
          dpi = 0.002*pi;
        }
    }
}

//================================
// phasediagram::checkResult
//================================
// erase not calculated points
void real_gas_models::phasediagram::checkResult(
            std::shared_ptr<binodalpoints> &bdp) {
  size_t sizebdp = bdp->t.size();
  for (size_t i = 0; i < sizebdp; ++i) {
      if (bdp->t[i] < -0.5) {
          eraseElements(bdp, i);
          --i;
          --sizebdp;
        }
    }
  // testing on negative values in vectors
  searchNegative(bdp, bdp->p);
  searchNegative(bdp, bdp->vLeft);
  searchNegative(bdp, bdp->vRigth);
}

//================================
// phasediagram::eraseElements
//================================

void real_gas_models::phasediagram::eraseElements(
    std::shared_ptr<binodalpoints> &bdp, const size_t i) {
  bdp->t.erase(bdp->t.begin() + i);
  bdp->p.erase(bdp->p.begin() + i);
  bdp-> vLeft.erase(bdp->vLeft.begin() + i);
  bdp->vRigth.erase(bdp->vRigth.begin() + i);
}

//================================
// phasediagram::searchNegative
//================================

void real_gas_models::phasediagram::searchNegative(
    std::shared_ptr<binodalpoints> &bdp, std::deque<double> &v) {
  auto hasnegative =[](std::deque<double>& vec) {
    return vec.end()-std::find_if(vec.begin(), vec.end(),
        std::bind2nd(std::less_equal<double>(), 0.0));};
  int hasneg = 1;
  int vecsize = v.size();
  while (true) {
      if ((hasneg = hasnegative(v))) {
          eraseElements(bdp, vecsize - hasneg);
          --vecsize;
          continue;
        }
      break;
    }
}

//================================
// phasediagram ctor
//================================

real_gas_models::phasediagram::phasediagram() {
  assert(lineIntegrate_.size() == initialize_.size());
}

//================================
// phasediagram::getCalculated
//================================

real_gas_models::phasediagram &real_gas_models::phasediagram::getCalculated() {
  static phasediagram phd;
  return phd;
}

//================================
// phasediagram::getBinodalPoints
//================================

real_gas_models::binodalpoints
real_gas_models::phasediagram::getBinodalPoints(double VK, double PK,
                                  double TK, modelName mn, double acentric) {
  bool isValid = true;
  for (auto &x : {VK, PK, TK, acentric})
    isValid &= ((x > 0.0) && std::isfinite(x));
  if (!isValid)
    throw modelExceptions(
        "phasediagram::getBinodalPoints get incorrect data: V_K, P_K, T_K \
         or acentric_factor <= 0.0 or is NaN");
  uniqueMark um {(size_t)mn, acentric};
  if (calculated_.find(um) == calculated_.end()) {
      std::lock_guard<std::mutex> lg(mtx);
      if (calculated_.find(um) == calculated_.end()) {
          std::shared_ptr<binodalpoints> bdp(new binodalpoints());
          try {
            calculateBinodal(bdp, mn, acentric);
          } catch(std::out_of_range&) {
            std::cout << "OUT_OF_RANGE exception in phasediagram::calculateBinodal"
                      << std::endl;
            throw;
          } catch(modelExceptions& e) {
            std::cout << e.what() << std::endl;
            throw;
          }
          checkResult(bdp);
          bdp->p.push_front(1.0);
          bdp->t.push_front(1.0);
          bdp->vLeft.push_front(1.0);
          bdp->vRigth.push_front(1.0);
          calculated_.emplace(um, bdp);
        }
    }
  binodalpoints bp((*calculated_.find(um)).second.operator *());
  auto f = [] (std::deque<double> &vec, double K) {
    std::transform(vec.begin(), vec.end(), vec.begin(),
        std::bind1st(std::multiplies<double>(), K));};
  f(bp.vLeft, VK);
  f(bp.vRigth, VK);
  f(bp.p, PK);
  f(bp.t, TK);
  return bp;
}

//================================
// phasediagram::eraseBinodalPoints
//================================

void real_gas_models::phasediagram::eraseBinodalPoints(modelName mn,
                                                    double acentric) {
  std::lock_guard<std::mutex> lg(mtx);
  auto iter = calculated_.find({(size_t)mn, acentric});
  if (iter != calculated_.end())
    calculated_.erase(iter);
}

//================================
// uniqueMark::operator<
//================================

bool real_gas_models::operator< (const phasediagram::uniqueMark& lum,
                                 const phasediagram::uniqueMark& rum) {
  return std::tie(lum.mn, lum.acentricfactor) <
      std::tuple<size_t, double>(rum.mn, rum.acentricfactor + 0.0001) &&
         std::tie(rum.mn, rum.acentricfactor) >
      std::tuple<size_t, double>(lum.mn, lum.acentricfactor + 0.0001);
}

//================================
// binodalpoints ctor
//================================

real_gas_models::binodalpoints::binodalpoints()
  :vLeft(t.size()), vRigth(t.size()), p(t.size()) {}
