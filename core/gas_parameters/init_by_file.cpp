#include "init_by_file.h"

#include <assert.h>

GasParameters *InitGasParameters::InitGasParameters(
    const std::string &filename) {
  assert(0);
  if (filename.empty()) {
    return nullptr;
  }
}

std::vector<GasParameters *> InitGasParameters::InitGasParameters(
    const std::vector<std::string> *files) {
  assert(0);
  return NULL;
}
