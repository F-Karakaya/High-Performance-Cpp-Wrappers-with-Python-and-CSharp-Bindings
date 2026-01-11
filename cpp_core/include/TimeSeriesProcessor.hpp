#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>


#ifdef _WIN32
#ifdef CPP_CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#else
#define CORE_API __attribute__((visibility("default")))
#endif

namespace Core {

/**
 * @brief High-performance processor for time-series data.
 *
 * Demonstrates RAII, const-correctness, and explicit memory management.
 */
class CORE_API TimeSeriesProcessor {
public:
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  TimeSeriesProcessor() = default;
  ~TimeSeriesProcessor() = default;

  // Delete copy, allow move
  TimeSeriesProcessor(const TimeSeriesProcessor &) = delete;
  TimeSeriesProcessor &operator=(const TimeSeriesProcessor &) = delete;
  TimeSeriesProcessor(TimeSeriesProcessor &&) noexcept = default;
  TimeSeriesProcessor &operator=(TimeSeriesProcessor &&) noexcept = default;

  /**
   * @brief Load data from a vector (copy).
   *
   * @param data Input data.
   */
  void LoadData(const std::vector<double> &data);

  /**
   * @brief Load data from a raw pointer (zero-copy view if possible, else
   * copy). For this implementation we will copy to internal storage for
   * ownership safety, but API design allows for future view-based optimization.
   *
   * @param data Pointer to data array.
   * @param size Number of elements.
   */
  void LoadData(const double *data, size_t size);

  /**
   * @brief Calculates Simple Moving Average (SMA).
   *
   * @param window_size Size of the moving window.
   * @return std::vector<double> SMA values. Result size = data_size -
   * window_size + 1.
   * @throws std::invalid_argument if window_size is invalid.
   */
  std::vector<double> CalculateMovingAverage(size_t window_size) const;

  /**
   * @brief Calculates Volatility (Standard Deviation) of the entire series.
   *
   * @return double Volatility value.
   */
  double CalculateVolatility() const;

  /**
   * @brief Detects anomalies based on threshold (Z-score approach relative to
   * mean).
   *
   * @param z_score_threshold Threshold in standard deviations.
   * @return std::vector<size_t> Indices of anomalies.
   */
  std::vector<size_t> FindAnomalies(double z_score_threshold) const;

  /**
   * @brief Returns the current data size.
   */
  size_t GetSize() const;

private:
  std::vector<double> data_;
};

} // namespace Core
