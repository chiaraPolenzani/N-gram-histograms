import pandas as pd
import matplotlib.pyplot as plt

#df = pd.read_csv("build/speedup_variable_threads.csv")

threads = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24]
speedup = [1.63, 2.42, 2.84, 3.02, 2.98, 2.83, 2.77, 2.98, 2.97, 3.00, 2.92, 2.98]

plt.figure(figsize=(8, 6))
plt.plot(threads, speedup, marker='o', linestyle='-', color='b', label="Speedup")
plt.axhline(y=max(speedup), color='r', linestyle='--', label=f"Max Speedup = {max(speedup):.2f}")
#plt.plot(df["Threads"], df["Speedup"], marker='o', linestyle='-', color='b', label="Speedup")
#plt.axhline(y=max(df["Speedup"]), color='r', linestyle='--', label="Max Speedup")

plt.xlabel("Number of threads")
plt.ylabel("Speedup")
plt.title("Speedup varying the number of threads")
plt.legend()
plt.grid(True)
plt.savefig("speedup_threads.png")
plt.show()

