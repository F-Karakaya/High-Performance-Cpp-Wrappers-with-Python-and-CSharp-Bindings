#include "CHelpers.hpp"
#include "../../cpp_core/include/TimeSeriesProcessor.hpp"
#include <algorithm>
#include <cstring>
#include <vector>


extern "C" {

TSP_Handle TSP_Create() { return new Core::TimeSeriesProcessor(); }

void TSP_Destroy(TSP_Handle handle) {
  if (handle) {
    delete static_cast<Core::TimeSeriesProcessor *>(handle);
  }
}

void TSP_LoadData(TSP_Handle handle, const double *data, int size) {
  if (handle && data && size > 0) {
    auto *processor = static_cast<Core::TimeSeriesProcessor *>(handle);
    processor->LoadData(data, static_cast<size_t>(size));
  }
}

double *TSP_CalculateMovingAverage(TSP_Handle handle, int window_size,
                                   int *out_size) {
  if (!handle || !out_size)
    return nullptr;
  auto *processor = static_cast<Core::TimeSeriesProcessor *>(handle);

  try {
    std::vector<double> result =
        processor->CalculateMovingAverage(static_cast<size_t>(window_size));
    *out_size = static_cast<int>(result.size());

    // Allocate memory that can be passed to C# (and must be freed later)
    double *out_array = new double[result.size()];
    std::memcpy(out_array, result.data(), result.size() * sizeof(double));
    return out_array;
  } catch (...) {
    *out_size = 0;
    return nullptr;
  }
}

double TSP_CalculateVolatility(TSP_Handle handle) {
  if (!handle)
    return 0.0;
  auto *processor = static_cast<Core::TimeSeriesProcessor *>(handle);
  return processor->CalculateVolatility();
}

size_t *TSP_FindAnomalies(TSP_Handle handle, double z_score_threshold,
                          int *out_size) {
  if (!handle || !out_size)
    return nullptr;
  auto *processor = static_cast<Core::TimeSeriesProcessor *>(handle);

  try {
    std::vector<size_t> result = processor->FindAnomalies(z_score_threshold);
    *out_size = static_cast<int>(result.size());

    size_t *out_array = new size_t[result.size()];
    std::memcpy(out_array, result.data(), result.size() * sizeof(size_t));
    return out_array;
  } catch (...) {
    *out_size = 0;
    return nullptr;
  }
}

void TSP_FreeDoubleArray(double *ptr) { delete[] ptr; }

void TSP_FreeSizeTArray(size_t *ptr) { delete[] ptr; }

int TSP_GetSize(TSP_Handle handle) {
  if (!handle)
    return 0;
  auto *processor = static_cast<Core::TimeSeriesProcessor *>(handle);
  return static_cast<int>(processor->GetSize());
}
}
