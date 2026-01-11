#include "../cpp_core/include/TimeSeriesProcessor.hpp"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

PYBIND11_MODULE(core_bindings, m) {
  m.doc() = "High-performance C++ Core bindings using pybind11";

  py::class_<Core::TimeSeriesProcessor>(m, "TimeSeriesProcessor")
      .def(py::init<>())
      .def(
          "load_data",
          [](Core::TimeSeriesProcessor &self, py::array_t<double> array) {
            py::buffer_info buf = array.request();
            if (buf.ndim != 1) {
              throw std::runtime_error("Number of dimensions must be one");
            }
            // In a real scenario, checks for stride/continuity would be here.
            // For now we assume standard contiguous double array.
            double *ptr = static_cast<double *>(buf.ptr);
            self.LoadData(ptr, buf.size);
          },
          "Load data from a numpy array (copies data into internal storage).")
      .def("calculate_moving_average",
           &Core::TimeSeriesProcessor::CalculateMovingAverage,
           py::arg("window_size"), "Calculate Simple Moving Average.")
      .def("calculate_volatility",
           &Core::TimeSeriesProcessor::CalculateVolatility,
           "Calculate Volatility (Standard Deviation).")
      .def("find_anomalies", &Core::TimeSeriesProcessor::FindAnomalies,
           py::arg("z_score_threshold"), "Find indices of anomalies.")
      .def("get_size", &Core::TimeSeriesProcessor::GetSize,
           "Get current data size.");
}
