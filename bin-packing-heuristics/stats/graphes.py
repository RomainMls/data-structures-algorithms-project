import matplotlib.pyplot as plt
import re

# Load and parse the data
file_path = "stats.txt"
with open(file_path, "r") as f:
    lines = f.readlines()

# Mapping short names to full names
name_map = {
    "next": "Next Fit",
    "worst": "Worst Fit",
    "best": "Best Fit",
    "first": "First Fit"
}

# Prepare data structures
data = {name: {"x": [], "time": [], "lost": []} for name in name_map.values()}

i = 0
while i < len(lines):
    line = lines[i].strip()
    if ";" in line:
        algo_short, x_value = line.split(";")
        algo_full = name_map[algo_short]
        x_value = int(x_value)
        avg_time_line = lines[i + 1].strip()
        avg_lost_line = lines[i + 2].strip()

        time_val = float(re.search(r"avg time = ([\d.]+)", avg_time_line).group(1))
        lost_val = int(re.search(r"avg lost = (\d+)", avg_lost_line).group(1))

        data[algo_full]["x"].append(x_value)
        data[algo_full]["time"].append(time_val)
        data[algo_full]["lost"].append(lost_val)

        i += 3  # move to next block
    else:
        i += 1  # skip unknown lines

# Plot time comparison
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

# Plot lost comparison
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

# Plot selected lost comparison
plt.figure(figsize=(12, 6))
for algo in ["Worst Fit", "Best Fit", "First Fit"]:
    plt.plot(data[algo]["x"], data[algo]["lost"], label=algo)
plt.title("Comparaison du gaspillage (Best/First/Worst)")
plt.xlabel("Nombre de fichiers")
plt.ylabel("Unités de gaspillage")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("lost_comparaison2.pdf")