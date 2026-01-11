using System;
using System.Runtime.InteropServices;
using System.Linq;

namespace HighPerformanceWrappers
{
    public class TimeSeriesProcessor : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TSP_Create();

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void TSP_Destroy(IntPtr handle);

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void TSP_LoadData(IntPtr handle, double[] data, int size);

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TSP_CalculateMovingAverage(IntPtr handle, int window_size, out int out_size);

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern double TSP_CalculateVolatility(IntPtr handle);

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TSP_FindAnomalies(IntPtr handle, double z_score_threshold, out int out_size);
        
        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void TSP_FreeDoubleArray(IntPtr ptr);

        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void TSP_FreeSizeTArray(IntPtr ptr);
        
        [DllImport("NativeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int TSP_GetSize(IntPtr handle);

        public TimeSeriesProcessor()
        {
            _handle = TSP_Create();
        }

        public void LoadData(double[] data)
        {
            if (data == null || data.Length == 0)
                throw new ArgumentException("Data cannot be empty");
            
            TSP_LoadData(_handle, data, data.Length);
        }

        public double[] CalculateMovingAverage(int windowSize)
        {
            int outSize;
            IntPtr ptr = TSP_CalculateMovingAverage(_handle, windowSize, out outSize);
            
            if (ptr == IntPtr.Zero)
            {
                if (outSize == 0) return new double[0]; 
                throw new Exception("Native calculation failed.");
            }

            try
            {
                double[] result = new double[outSize];
                Marshal.Copy(ptr, result, 0, outSize);
                return result;
            }
            finally
            {
                TSP_FreeDoubleArray(ptr);
            }
        }

        public double CalculateVolatility()
        {
            return TSP_CalculateVolatility(_handle);
        }

        public long[] FindAnomalies(double zScoreThreshold)
        {
            int outSize;
            IntPtr ptr = TSP_FindAnomalies(_handle, zScoreThreshold, out outSize);

             if (ptr == IntPtr.Zero)
            {
                if (outSize == 0) return new long[0];
                 throw new Exception("Native calculation failed.");
            }

            try
            {
                // Native size_t can be 64-bit (long) or 32-bit (int) depending on arch.
                // Assuming 64-bit for modern high-perf systems.
                long[] result = new long[outSize];
                Marshal.Copy(ptr, result, 0, outSize);
                return result;
            }
            finally
            {
                TSP_FreeSizeTArray(ptr);
            }
        }

        public int Size => TSP_GetSize(_handle);

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_handle != IntPtr.Zero)
                {
                    TSP_Destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~TimeSeriesProcessor()
        {
            Dispose(false);
        }
    }
}
