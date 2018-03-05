#ifndef SRC_MODEL_GENERAL_H_
#define SRC_MODEL_GENERAL_H_

#include <functional>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/array.hpp>

#include "gas_description.h"
#include "phase_diagram.h"

namespace real_gas_models {
  typedef boost::array<float, 2> difresult_t;
  typedef std::function<void (const difresult_t &x, difresult_t &dxdt,
                                                  double t)> difEquat_t;

  // change "parameters_" type among Igasparameters realizations
  class gasparametersConverter;

  //================================
  // DerivateFunctor
  //================================
  // get the functor of the model for calculating gas dynamic equation
  class DerivateFunctor {
  public:
    virtual void getFunctor(class idealGas &mg)       = 0;
    virtual void getFunctor(class Redlich_Kwong2 &mg) = 0;
    virtual void getFunctor(class Peng_Robinson &mg)  = 0;
    virtual ~DerivateFunctor() {}
  };

  //================================
  // modelGeneral
  //================================

  class modelGeneral : boost::noncopyable {
  protected:
    friend class gasparametersConverter;

    std::unique_ptr<Igasparameters> parameters_;
    modelName phasediag_model_;
    binodalpoints bp_;

    modelGeneral(modelName mn, std::shared_ptr<constgasparameters> &cgp);

  protected:
    state_phase setState_phase(float v, float p, float t);
    int  setState_phasesub(float p);
    void setParameters(float v, float p, float t);
    void setDynamicParameters();
    void setStaticParameters();

  public:
    virtual bool isValid()                         const = 0;
    virtual void dynamicflowAccept(DerivateFunctor &df)  = 0;
    virtual void setVolume(float p, float t)             = 0;
    virtual void setPressure(float v, float t)           = 0;
    virtual float getVolume(float p, float t)      const = 0;
    virtual float getPressure(float v, float t)    const = 0;

    float getVolume()                     const;
    float getPressure()                   const;
    float getTemperature()                const;
    float getCV()                         const;
    float getAcentric()                   const;
    float getAdiabatic()                  const;
    float getT_K()                        const;
    state_phase getState()                const;
    parameters getParametersCopy()        const;     
    std::shared_ptr<constgasparameters> getConstParameters() const;
    virtual ~modelGeneral() {}
  };

  //================================
  // gasparametersConverter
  //================================

  class gasparametersConverter {
    modelGeneral* md_;

  public:
    explicit gasparametersConverter(modelGeneral *md);
    ~gasparametersConverter();
  };
}  // namespace real_gas_models

#endif  // SRC_MODEL_GENERAL_H_
