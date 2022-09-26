import sys

if not hasattr(sys, 'argv'):
    sys.argv = []

def init_pybind_call():
    if not sys.argv:
        sys.argv.append("python")

def print_hello():
    init_pybind_call()
    print("Hello World")

def return_hello():
    init_pybind_call()
    return "Hello World"
