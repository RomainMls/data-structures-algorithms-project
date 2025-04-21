import matplotlib.pyplot as plt
import re

# Load and parse the data
file_path = "stats.txt"
with open(file_path, "r") as f:
    lines = f.readlines()

# Mapping short names to full names
name_map = {
    "bpnextfit": "Next Fit",
    "bpworstfit": "Worst Fit",
    "bpbestfit": "Best Fit",
    "bpfirstfit": "First Fit"
}

# Prepare data structures
data = {name: {"x": [], "time": [], "lost": []} for name in name_map.values()}

current_algo = ""
for line in lines:
    line = line.strip()
    if line.startswith("./") or line.startswith("bp"):
        raw_algo = line.split()[0].replace("./", "")
        current_algo = name_map[raw_algo]
        x_value = int(line.split()[1])
        data[current_algo]["x"].append(x_value)
    elif "avg time" in line:
        time_val = float(re.search(r"avg time = ([\d.]+)", line).group(1))
        data[current_algo]["time"].append(time_val)
    elif "avg lost" in line:
        lost_val = int(re.search(r"avg lost = (\d+)", line).group(1))
        data[current_algo]["lost"].append(lost_val)

# Plot avg time
plt.figure(figsize=(12, 6))
for algo, values in data.items():
    plt.plot(values["x"], values["time"], label=algo)
plt.title("Comparaison du temps de calcul")
plt.xlabel("Nombre de fichiers")
plt.ylabel("Temps (secondes)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("time_comparaison.pdf")

# Plot avg lost
plt.figure(figsize=(12, 6))
for algo, values in data.items():
    plt.plot(values["x"], values["lost"], label=algo)
plt.title("Comparaison du gaspillage")
plt.xlabel("Nombre de fichiers")
plt.ylabel("Unités de gaspillage")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("lost_comparaison.pdf")

# Plot avg lost for only Best Fit and First Fit
plt.figure(figsize=(12, 6))
for algo in ["Best Fit", "First Fit"]:
    plt.plot(data[algo]["x"], data[algo]["lost"], label=algo)
plt.title("Comparaison du gaspillage")
plt.xlabel("Nombre de fichiers")
plt.ylabel("Unités de gaspillage")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("lost_comparaison_bestfit_firstfit.pdf")
