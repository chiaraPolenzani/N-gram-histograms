import pandas as pd
import matplotlib.pyplot as plt

# Carica il CSV
df = pd.read_csv("build/speedup_variable_size.csv")

# Plot
plt.figure(figsize=(10, 6))
plt.plot(df["Text Size(MB)"], df["Speedup"], marker='o', linestyle='-', color='red')
plt.title("Speedup trend with respect of the size of the texts set")
plt.xlabel("Texts set size (MB)")
plt.ylabel("Speedup")
plt.grid(True)
plt.savefig("speedup_variable_size.png")
plt.show()
