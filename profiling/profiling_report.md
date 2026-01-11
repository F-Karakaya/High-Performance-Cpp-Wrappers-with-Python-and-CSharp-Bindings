# Profiling and Performance Analysis

## Methodology
Performance profiling was designed to measure the overhead introduced by the language wrappers (Python and C#) compared to the raw C++ Core performance.

### Tools Used
- **C++**: Google Benchmark for micro-benchmarks.
- **Python**: `timeit` module and `cProfile` for Python-side overhead.
- **C#**: `System.Diagnostics.Stopwatch` for high-resolution timing.

## Theoretical Performance Analysis

### C++ Core
The `TimeSeriesProcessor` uses `std::vector` contiguous memory, which is cache-friendly.
- **Moving Average**: O(N) complexity. The implementation uses a sliding window optimized with a running sum, reducing re-computations.
- **Volatility**: Two-pass algorithm (mean calculation then variance). O(N).
- **Memory**: Vectors are used to ensure RAII.

### Python Wrapper (pybind11)
- **Data Loading**: We support `py::array_t` which allows accessing NumPy arrays directly.
    - *Zero-Copy*: If the NumPy array is contiguous and double-precision, we can read it directly. However, the current `LoadData` implementation performs a copy to ensure ownership and safety within the C++ class. This introduces an O(N) overhead during the `load_data` call.
    - *Future Optimization*: Implementing a view-based `LoadViewer` would eliminate this copy but requires careful lifetime management.
- **Function Calls**: Pybind11 introduces a small overhead (typically <50ns) per call. For computationally heavy tasks like `CalculateMovingAverage` on large datasets, this overhead is negligible (<1%).

### C# Wrapper (P/Invoke)
- **Data Loading**: `TSP_LoadData` accepts a pointer. The C# `Marshal.Copy` or pinning (`fixed`) is required to pass data.
    - Our implementation uses `double[]` and marshaling, which copies data. P/Invoke marshaling cost is proportional to data size.
- **Return Values**: Returning arrays from C++ to C# is tricky. We allocate memory in C++ using `new`, pass the pointer to C#, copy it into a managed array using `Marshal.Copy`, and then free the C++ memory. This results in double allocation and copy.
    - *Optimization*: Using `unsafe` code and working with pointers directly in C# would avoid the copy to managed arrays, but reduces safety.

## Expected Bottlenecks
1.  **Memory Bandwidth**: For simple calculations like SMA, memory bandwidth will be the bottleneck for large arrays.
2.  **Boundary Crossing**: For very small datasets or frequent calls, the wrapper overhead (marshalling) will dominate.

## Profiling Results (Actual)

Run on (12 X 2592 MHz CPU s)
L3 Cache: 12MB

| Operation | Input Size | C++ Time (ns) | Analysis |
|-----------|------------|---------------|----------|
| SMA       | 1,024      | 1,319         | ~1.3 µs (L1 cache hit) |
| SMA       | 262,144    | 766,863       | ~0.7 ms (Memory bandwidth limited) |
| SMA       | 8,388,608  | 26,956,520    | ~27.0 ms (Linear scaling O(N)) |
| Volatility| 1,024      | 1,997         | ~2.0 µs |
| Volatility| 8,388,608  | 17,354,741    | ~17.4 ms |

### Wrapper Overhead Observations
- **Python**: Function call overhead is <1µs via pybind11. Data loading for 8M doubles (~64MB) involves a copy step, taking approx 10-15ms additional time, making it the dominant factor for single-shot processing.
- **C#**: P/Invoke overhead is negligible (<100ns). Data marshalling (array copy) has similar characteristics to Python, dominating the cost for large transfers.

### Conclusion
The C++ core delivers predictable linear performance. The wrappers provide convenient access but introduce memory copy overheads. Critical high-frequency paths should batch data or use zero-copy mechanisms (e.g. `unsafe` pointers in C#, buffer protocol in Python w/o copy) if latency < 1ms is required for large datasets.
