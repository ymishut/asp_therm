#ifndef _CORE__GAS_PARAMETERS__GAS_MIX_INIT_H_
#define _CORE__GAS_PARAMETERS__GAS_MIX_INIT_H_

#include "gas_description.h"

#include <map>
#include <utility>

/*
 * Файл для инициализации параметров газовых смесей
 *   по параметрам их составляющих(с учетом доли)
*/

// Не имеет слысла определять все составляющие газовой
//   смеси. 97% составляющихбудет достаточно
// И не забудем ошибку вычисления, или округления,
//   отведём ей 1%
// Итого проверяем тождество :
//   |0.99 - ${summ_of_components}| < 0.02
#define GAS_MIX_PERCENT_EPS  0.02

typedef std::pair<const const_parameters &, const dyn_parameters &>
    const_dyn_parameters;
//                молярная доля, % ; параметры доли
typedef std::multimap<const double, const_dyn_parameters> parameters_mix;


std::pair<const_parameters *, dyn_parameters *>
    get_parameters_of_mix(parameters_mix cgp_mix);

#endif  // _CORE__GAS_PARAMETERS__GAS_MIX_INIT_H_
