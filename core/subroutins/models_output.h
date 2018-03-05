#ifndef SRC_MODELS_OUTPUT_H_
#define SRC_MODELS_OUTPUT_H_

#include <ostream>
#include <iomanip>

//================================
// formatted_output
//================================

class formatted_output {
private:
  int width     = 11,
      precision = 8;
  std::ostream& stream_obj;

public:
  explicit formatted_output(std::ostream& obj)
    :stream_obj(obj) {}

  template<class T>
  formatted_output& operator<< (const T& output) {
    stream_obj << std::setprecision(precision) << std::setw(width)
               << output << " ";
    return *this;
  }

  inline formatted_output& operator<< (std::ostream& (*func)(std::ostream&)) {
    func(stream_obj);
    return *this;
  }
};

#endif  // SRC_MODELS_OUTPUT_H_

