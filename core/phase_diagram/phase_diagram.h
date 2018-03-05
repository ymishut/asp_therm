#ifndef SRC_PHASE_DIAGRAM_H_
#define SRC_PHASE_DIAGRAM_H_

#include <deque>
#include <memory>
#include <mutex>
#include <functional>
#include <map>
#include <vector>

#include <boost/noncopyable.hpp>

#include "phase_diagram_models.h"

namespace real_gas_models {
  class phasediagram;

  //================================
  // modelName  enum
  //================================

  enum class modelName {REDLICH_KWONG2 = 0, PENG_ROBINSON};


  //================================
  // binodalpoints  (class)
  //================================

  class binodalpoints {                           
    friend class phasediagram;
    binodalpoints();

  public:
    std::deque<double> t = {0.97, 0.95, 0.92, 0.9, 0.87, 0.85,
                            0.8, 0.75, 0.7, 0.6, 0.5},
                       vLeft,
                       vRigth,
                       p;
  };

  //================================
  // phasediagram
  //================================

  /// class calculate points on diagram liquid-steam-gas
  /// for more information visit:
  ///    https://en.wikipedia.org/wiki/Maxwell_construction
  ///

  class phasediagram: boost::noncopyable {
    //================================
    // uniqueMark
    //================================

    struct uniqueMark {
      size_t mn;
      double acentricfactor;
    };

    friend bool operator< (const phasediagram::uniqueMark& lum,
                           const phasediagram::uniqueMark& rum);

    std::mutex mtx;
    std::map<uniqueMark, std::shared_ptr<binodalpoints>> calculated_;
    std::vector<std::function<double(double, double, double, double)>>
      lineIntegrate_ {lineIntegrateRK2(), lineIntegratePR()};
    std::vector<std::function<void(std::vector<double>&, double, double, double)>>
      initialize_ {initializeRK2(), initializePR()};

  private:
    void calculateBinodal(std::shared_ptr<binodalpoints> &bdp, modelName mn,
                                                           double acentric);

    void checkResult(std::shared_ptr<binodalpoints> &bdp);

    void eraseElements(std::shared_ptr<binodalpoints> &bdp, const size_t i);

    void searchNegative(std::shared_ptr<binodalpoints> &bdp, std::deque<double> &v);

    phasediagram();

  public:
    static phasediagram& getCalculated();

    binodalpoints getBinodalPoints(double VK, double PK, double TK,
                                     modelName mn, double acentric);

    void eraseBinodalPoints(modelName mn, double acentric);
  };

  bool operator< (const phasediagram::uniqueMark& lum,
                  const phasediagram::uniqueMark& rum);
}  // namespace real_gas_models

#endif  // SRC_PHASE_DIAGRAM_H_

