#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "py_interface.h"

namespace py = pybind11;

PYBIND11_MODULE(py_interface_pybind, m) {
    pybind11::class_<PyInterface>(m, "PyInterface")
        .def(pybind11::init())
        .def("test_func", &PyInterface::test_func);
}
