#include "TimeSeriesProcessor.hpp"
#include <algorithm>
#include <cmath>
#include <execution>
#include <numeric>


namespace Core {

void TimeSeriesProcessor::LoadData(const std::vector<double> &data) {
  if (data.empty()) {
    throw std::invalid_argument("Input data cannot be empty.");
  }
  data_ = data;
}

void TimeSeriesProcessor::LoadData(const double *data, size_t size) {
  if (!data || size == 0) {
    throw std::invalid_argument("Invalid data pointer or size.");
  }
  data_.assign(data, data + size);
}

std::vector<double>
TimeSeriesProcessor::CalculateMovingAverage(size_t window_size) const {
  if (window_size == 0 || window_size > data_.size()) {
    throw std::invalid_argument("Invalid window size.");
  }

  size_t result_size = data_.size() - window_size + 1;
  std::vector<double> result(result_size);

  double current_sum = 0.0;
  // Initial window
  for (size_t i = 0; i < window_size; ++i) {
    current_sum += data_[i];
  }
  result[0] = current_sum / window_size;

  // Sliding window
  for (size_t i = 1; i < result_size; ++i) {
    current_sum += data_[i + window_size - 1] - data_[i - 1];
    result[i] = current_sum / window_size;
  }

  return result;
}

double TimeSeriesProcessor::CalculateVolatility() const {
  if (data_.size() < 2)
    return 0.0;

  double mean = std::accumulate(data_.begin(), data_.end(), 0.0) / data_.size();

  double sq_sum = std::accumulate(data_.begin(), data_.end(), 0.0,
                                  [mean](double acc, double val) {
                                    return acc + (val - mean) * (val - mean);
                                  });

  return std::sqrt(sq_sum / (data_.size() - 1)); // Sample standard deviation
}

std::vector<size_t>
TimeSeriesProcessor::FindAnomalies(double z_score_threshold) const {
  std::vector<size_t> indices;
  if (data_.size() < 2)
    return indices;

  double mean = std::accumulate(data_.begin(), data_.end(), 0.0) / data_.size();
  double vol = CalculateVolatility();

  if (vol == 0.0)
    return indices;

  for (size_t i = 0; i < data_.size(); ++i) {
    double z_score = std::abs(data_[i] - mean) / vol;
    if (z_score > z_score_threshold) {
      indices.push_back(i);
    }
  }
  return indices;
}

size_t TimeSeriesProcessor::GetSize() const { return data_.size(); }

} // namespace Core
