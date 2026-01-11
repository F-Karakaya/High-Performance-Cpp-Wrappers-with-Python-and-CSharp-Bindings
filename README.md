# High-Performance C++ Wrappers with Python and C# Bindings

## Overview
This repository demonstrates the design and implementation of a high-performance C++ core library (`TimeSeriesProcessor`) and its integration into Python and C# ecosystems. It serves as a reference for building production-grade native extensions, handling cross-language memory management, and designing clean, robust APIs.

## Directory Structure
- **`cpp_core/`**: The native C++17 library containing the core business logic, optimized for performance.
- **`python_wrapper/`**: Python bindings using `pybind11`, enabling seamless usage with NumPy.
- **`csharp_wrapper/`**: C# .NET 6 wrapper using P/Invoke to interact with the native library.
- **`profiling/`**: Analysis of performance characteristics and wrapper overhead.
- **`benchmarks/`**: C++ micro-benchmarks verifying the core performance.

## Core Design
The C++ core is designed with modern C++ principles:
- **RAII**: Resource acquisition is initialization; memory is managed automatically via `std::vector` and smart pointers where appropriate.
- **Const Correctness**: rigorous usage of `const` to ensure thread-safety potential and code clarity.
- **Error Handling**: Standard exceptions (`std::invalid_argument`) are used and translated to language-specific exceptions in the wrappers.

## Wrappers
### Python
- **Technology**: `pybind11` support efficient bindings.
- **Integration**: Supports loading data from NumPy arrays. Exceptions are correctly propagated.

### C#
- **Technology**: P/Invoke (Platform Invocation Services).
- **Safety**: The managed wrapper implements `IDisposable` to ensure native resources are freed (Deterministically via `Dispose` or safely via Finalizer).

## Build Instructions

### Prerequisites
- CMake 3.14+
- C++ Compiler (MSVC, GCC, or Clang) supporting C++17
- Python 3.8+ (with `pybind11` installed)
- .NET 6.0 SDK or later

### Build Steps
1.  **Build C++ Core and NativeLib**:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build . --config Release
    ```
2.  **Build Python Wrapper**:
    ```bash
    cd python_wrapper
    python setup.py build_ext --inplace
    ```
3.  **Run C# application**:
    ```bash
    cd csharp_wrapper/ManagedAPI
    dotnet run
    ```

## Performance
Performance is a primary concern. The core algorithms are O(N) and memory-efficient.
Refer to `profiling/profiling_report.md` for detailed analysis.

## Verification
**Status: PASSED** (Tested on Windows 11 x64, VS 2026/2022)

- **C++ Benchmarks**: Confirmed linear scaling O(N).
  - SMA 8M items: ~27ms
  - Volatility 8M items: ~17ms
- **Python Integration**: Verified. `test_wrapper.py` successfully loads and computes using the C++ core.
- **C# Integration**: Verified. `ManagedAPI` successfully calls NativeLib.dll via P/Invoke.

See `profiling/profiling_report.md` for detailed results.

## 👤 Author

**Furkan Karakaya**  
AI & Computer Vision Engineer  

📧 se.furkankarakaya@gmail.com  

⭐ If this project helps your workflow or research, consider starring the repository.