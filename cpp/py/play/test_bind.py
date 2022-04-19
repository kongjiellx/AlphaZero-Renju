import cpp.src
print(dir(cpp.src))
from cpp.src.py_interface_pybind import PyInterface, Player



if __name__ == "__main__":
    print("Hello World.")
    a = PyInterface()
    print(type(a.self_move()))