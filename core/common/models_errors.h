#ifndef _CORE__COMMON__MODELS_ERRORS_H_
#define _CORE__COMMON__MODELS_ERRORS_H_

#include <stdint.h>

#define ERROR_TYPE       uint32_t
#define ERR_MSG_MAX_LEN  200

// error type
#define ERR_MASK_TYPE    0x000f

#define ERR_SUCCESS      0x0000
#define ERR_FILEIO       0x0001
#define ERR_CALCULATE    0x0002
#define ERR_STRING       0x0003
#define ERR_INIT         0x0004

// type gas / gas_mix
#define ERR_MASK_GAS_MIX 0x00f0
// посути просто маркер
//   смеси газов
// #define ERR_GAS_PURE     0x0000
#define ERR_GAS_MIX      0x0010

// error subtype
#define ERR_MASK_SUBTYPE 0x0f00
// fileio errors
#define ERR_FILE_IN      0x0100
#define ERR_FILE_OUT     0x0200

// calculate errors
#define ERR_CALC_GAS_P   0x0100
#define ERR_CALC_PHASE   0x0200
#define ERR_CALC_MODEL   0x0300

// string errors
#define ERR_STR_MAX_LEN  0x0100
#define ERR_STR_PARSE    0x0200

// init errors
#define ERR_INIT_ZERO    0x0100
#define ERR_INIT_NULLP   0x0200


// ERR_SUCCESS as default
void set_error_code(ERROR_TYPE err);
ERROR_TYPE get_error_code();
// set err_code to SUCCESS and
//   forget setted error message
void reset_error();

// replace custom errormsg with *msg
void set_error_message(const char *msg);
// add to custom errormsg *msg
void add_to_error_msg(const char *msg);
char *get_error_message();

#endif  // _CORE__COMMON__MODELS_ERRORS_H_
