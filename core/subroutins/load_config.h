#ifndef _CORE__SUBROUTINS__LOAD_CONFIG_H_
#define _CORE__SUBROUTINS__LOAD_CONFIG_H_

//================================
// SetConfigure
//================================
class SetConfigure {
  static bool loaded;

  static void loadConf();
  SetConfigure() {}

public:
  static void setConf();
};

#endif  // _CORE__SUBROUTINS__LOAD_CONFIG_H_

