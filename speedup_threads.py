import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("build/speedup_variable_threads.csv")

plt.figure(figsize=(8, 6))
plt.plot(df["Threads"], df["Speedup"], marker='o', linestyle='-', color='b', label="Speedup")
plt.axhline(y=max(df["Speedup"]), color='r', linestyle='--', label="Max Speedup")

plt.xlabel("Number of threads")
plt.ylabel("Speedup")
plt.title("Speedup varying the number of threads")
plt.legend()
plt.grid(True)
plt.savefig("speedup_threads.png")
plt.show()

