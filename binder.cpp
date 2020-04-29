#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "QHull.h"

namespace py = pybind11;

PYBIND11_MODULE(QHull, m) {
  m.def("calc", [](const Eigen::MatrixXd& points) { return QHull(points); });
}
