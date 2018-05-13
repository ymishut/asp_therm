#ifndef _CORE__MODELS__MODEL_GENERAL_H_
#define _CORE__MODELS__MODEL_GENERAL_H_

#include "target.h"

#include "gas_description.h"
#include "gas_description_static.h"
#include "gas_mix_init.h"
#include "phase_diagram.h"

#include <boost/noncopyable.hpp>

#include <array>
#include <functional>
#include <memory>

/* // flow dynamic modeling
typedef std::array<double, 2> difresult_t;
typedef std::function<void (const difresult_t &x, difresult_t &dxdt,
    double t)> difEquat_f;
*/
// класс для восстановления типа модели
// DEVELOP
//   Acceptor
// get the functor of the model for calculating gas dynamic equation
class DerivateFunctor {
public:
  virtual void getFunctor(class IdealGas &mg)       = 0;
  virtual void getFunctor(class Redlich_Kwong2 &mg) = 0;
  virtual void getFunctor(class Peng_Robinson &mg)  = 0;
  virtual ~DerivateFunctor() {}
};

class modelGeneral : boost::noncopyable {
protected:
  // Храним указатель на GasParameters
  //   чтобы создать возможность использовать ООП для расчёта динамики газа
  // parameters_ must be a pointer for enable
  //   swap static-dynamic parameters
  std::unique_ptr<GasParameters> parameters_;
  // имя модели для расчёта точек бинодали
  // model for calculating phase diagram
  modelName phasediag_model_;
  // binodal points
  binodalpoints bp_;

 // dyn_params_update params_update_f_;

protected:
  modelGeneral(modelName mn, parameters prs, const_parameters cgp,
      dyn_parameters dgp, binodalpoints bp);

  modelGeneral(modelName mn, parameters prs, parameters_mix components,
      binodalpoints bp);

  state_phase setState_phase(double v, double p, double t);
  int32_t  setState_phasesub(double p);
  void setParameters(double v, double p, double t);
  void setDynamicParameters();
  void setStaticParameters();

  const GasParameters *getGasParameters() const;

public:
//  virtual double internal_energy_integral(
//      const parameters state) = 0;
  /// Функция обновления динамических параметров
  /// Update dynn_parameters function
  virtual void update_dyn_params(dyn_parameters &prev_state,
      const parameters new_state) = 0;

  // Модели имеют определенные границы применения
  //   метод isValid проверяет их
  // Models have application limits
  //  isValid - method for check this limits
  virtual bool IsValid()                         const = 0;
  virtual void DynamicflowAccept(DerivateFunctor &df)  = 0;

  virtual void SetVolume(double p, double t)           = 0;
  virtual void SetPressure(double v, double t)         = 0;
  virtual double GetVolume(double p, double t)   const = 0;
  virtual double GetPressure(double v, double t) const = 0;

  double GetVolume()                     const;
  double GetPressure()                   const;
  double GetTemperature()                const;
  double GetAcentric()                   const;
  double GetCV()                         const;
  double GetAdiabatic()                  const;
  double GetT_K()                        const;
  state_phase GetState()                 const;
  parameters GetParametersCopy()         const;
  const_parameters GetConstParameters()  const;

  virtual ~modelGeneral();
};

#endif  // ! _CORE__MODELS__MODEL_GENERAL_H_
