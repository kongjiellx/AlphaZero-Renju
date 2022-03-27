import cpp.src
print(dir(cpp.src))
from cpp.src.py_interface_pybind import PyInterface


if __name__ == "__main__":
    print("Hello World.")
    a = PyInterface()
    print(a.test_func())