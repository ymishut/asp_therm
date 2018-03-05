#ifndef SRC_LOAD_CONFIG_H_
#define SRC_LOAD_CONFIG_H_

namespace real_gas_models {
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
}  // namespace real_gas_models

#endif  // SRC_LOAD_CONFIG_H_

