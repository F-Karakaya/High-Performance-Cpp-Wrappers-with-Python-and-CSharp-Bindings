#include "TimeSeriesProcessor.hpp"
#include <benchmark/benchmark.h>
#include <random>
#include <vector>


static void BM_CalculateMovingAverage(benchmark::State &state) {
  Core::TimeSeriesProcessor processor;
  size_t data_size = state.range(0);

  // Setup random data
  std::vector<double> data(data_size);
  std::mt19937 gen(42);
  std::uniform_real_distribution<> dis(0.0, 100.0);
  for (size_t i = 0; i < data_size; ++i)
    data[i] = dis(gen);

  processor.LoadData(data);

  for (auto _ : state) {
    auto res = processor.CalculateMovingAverage(100);
    benchmark::DoNotOptimize(res);
  }
}

static void BM_CalculateVolatility(benchmark::State &state) {
  Core::TimeSeriesProcessor processor;
  size_t data_size = state.range(0);

  // Setup random data
  std::vector<double> data(data_size);
  std::mt19937 gen(42);
  std::uniform_real_distribution<> dis(0.0, 100.0);
  for (size_t i = 0; i < data_size; ++i)
    data[i] = dis(gen);

  processor.LoadData(data);

  for (auto _ : state) {
    double vol = processor.CalculateVolatility();
    benchmark::DoNotOptimize(vol);
  }
}

BENCHMARK(BM_CalculateMovingAverage)->Range(1024, 8 << 20);
BENCHMARK(BM_CalculateVolatility)->Range(1024, 8 << 20);

BENCHMARK_MAIN();
