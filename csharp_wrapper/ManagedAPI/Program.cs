using System;
using System.Linq;

namespace HighPerformanceWrappers
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("C# Wrapper Test Starting...");

            using (var processor = new TimeSeriesProcessor())
            {
                // Generate data
                double[] data = Enumerable.Range(0, 1000).Select(x => (double)x + Math.Sin(x)).ToArray();
                
                Console.WriteLine($"Loading {data.Length} data points...");
                processor.LoadData(data);

                // Test SMA
                int window = 10;
                var sma = processor.CalculateMovingAverage(window);
                Console.WriteLine($"SMA Calculated. Result size: {sma.Length}. First value: {sma[0]}");

                // Test Volatility
                var vol = processor.CalculateVolatility();
                Console.WriteLine($"Volatility: {vol}");
                
                // Test Anomalies (add some spikes)
                data[500] = 10000.0;
                processor.LoadData(data);
                var anomalies = processor.FindAnomalies(3.0);
                Console.WriteLine($"Found {anomalies.Length} anomalies. Index of first: {anomalies.FirstOrDefault()}");

            }
             Console.WriteLine("Test Completed Successfully.");
        }
    }
}
