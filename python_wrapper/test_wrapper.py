import sys
import os

# Add build path to sys.path to find the pyd file
# We need to find where setup.py build_ext --inplace put it.
# It should be in the current directory or build/lib...
sys.path.append(os.getcwd())
# Also try build/lib....
try:
    import core_bindings
    print("Module loaded successfully.")
except ImportError:
    # Try to find it in build subdirs
    for root, dirs, files in os.walk("build"):
        for file in files:
            if file.endswith(".pyd") or file.endswith(".so"):
                sys.path.append(root)
                break
    import core_bindings
    print("Module loaded from build path.")

import numpy as np

def test_wrapper():
    print("Testing Python Wrapper...")
    data = np.arange(1000, dtype=np.float64) + np.sin(np.arange(1000))
    
    processor = core_bindings.TimeSeriesProcessor()
    
    # Test Load
    processor.load_data(data)
    print(f"Loaded {processor.get_size()} items.")
    
    # Test SMA
    sma = processor.calculate_moving_average(10)
    print(f"SMA[0]: {sma[0]:.4f}")
    
    # Test Volatility
    vol = processor.calculate_volatility()
    print(f"Volatility: {vol:.4f}")
    
    # Test Anomalies
    data[500] = 10000.0
    processor.load_data(data)
    anomalies = processor.find_anomalies(3.0)
    print(f"Found {len(anomalies)} anomalies. First index: {anomalies[0]}")
    
    print("Python Verification Passed.")

if __name__ == "__main__":
    test_wrapper()
