import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("results.csv")

x = np.log2(df["SizeKB"]*1024)

plt.plot(x, df["Sequential(ns)"], marker='o', label="Sequential")
plt.plot(x, df["Random(ns)"], marker='s', label="Random")

plt.xlabel("log2(Working Set Size in Bytes)")
plt.ylabel("Time per Access (ns)")
plt.title("Memory Hierarchy Benchmark")
plt.grid(True)
plt.legend()
plt.show()