from setuptools import setup, Extension
import sys
import os

# Check if we are running on Windows
is_windows = sys.platform == 'win32'

# Define the extension module
ext_modules = [
    Extension(
        'core_bindings',
        ['bindings.cpp', '../cpp_core/src/TimeSeriesProcessor.cpp'],
        include_dirs=['../cpp_core/include'],
        language='c++',
        extra_compile_args=['/std:c++17', '/DCPP_CORE_EXPORTS'] if is_windows else ['-std=c++17', '-O3', '-DCPP_CORE_EXPORTS'],
    ),
]

# We need to install pybind11
try:
    import pybind11
    ext_modules[0].include_dirs.append(pybind11.get_include())
except ImportError:
    print("Warning: pybind11 not found. Please install it via 'pip install pybind11'")

setup(
    name='core_bindings',
    version='1.0',
    description='High-performance C++ Core bindings',
    ext_modules=ext_modules,
    zip_safe=False,
)
