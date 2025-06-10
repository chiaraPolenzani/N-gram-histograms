import pandas as pd
import matplotlib.pyplot as plt

# Carica il CSV
#df = pd.read_csv("build/speedup_variable_size.csv")
folder_size = [100, 200, 300, 400, 500, 600]
speedup = [3.04, 3.14, 3.12, 2.93, 2.80, 2.98]

# Plot
plt.figure(figsize=(10, 6))
plt.plot(folder_size, speedup, marker='o', linestyle='-', color='b', label="Speedup")
#plt.plot(df["Text Size(MB)"], df["Speedup"], marker='o', linestyle='-', color='red')
plt.title("Speedup trend with respect of the size of the texts set")
plt.xlabel("Texts set size (MB)")
plt.ylabel("Speedup")
plt.grid(True)
plt.savefig("speedup_variable_size.png")
plt.show()
