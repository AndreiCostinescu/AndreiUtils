import os
import sys
from scripts.hello_world_external import return_hello_string


if not hasattr(sys, 'argv'):
    sys.argv = []


def init_pybind_call():
    if not sys.argv:
        sys.argv.append("python")


def print_hello():
    init_pybind_call()
    print(return_hello_string())


def return_hello():
    print(sys.path)
    print(os.getcwd())
    init_pybind_call()
    return return_hello_string()
