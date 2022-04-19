#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "py_interface.h"


PYBIND11_MODULE(py_interface_pybind, m) {
    pybind11::class_<PyInterface>(m, "PyInterface")
        .def(pybind11::init())
        .def("test_func", &PyInterface::test_func)
        .def("init", &PyInterface::init)
        .def("new_game", &PyInterface::new_game)
        .def("enemy_move", &PyInterface::enemy_move)
        .def("self_move", &PyInterface::self_move);

    pybind11::enum_<Player>(m, "Player")
        .value("O", Player::O)
        .value("X", Player::X)
        .value("NONE", Player::NONE)
        .export_values();
}
