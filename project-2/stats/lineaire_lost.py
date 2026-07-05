import re
import numpy as np
from sklearn.linear_model import LinearRegression

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

# Function to compute slope and R²
def compute_regression(x, y):
    x = np.array(x).reshape(-1, 1)
    y = np.array(y)
    model = LinearRegression().fit(x, y)
    return model.coef_[0], model.score(x, y)

# Calculate and print results
for key in data:
    x = np.array(data[key]["x"])
    y = np.array(data[key]["lost"])

    # Linear
    slope_lin, r2_lin = compute_regression(x, y)
    # Logarithmic
    log_x = np.log(x)
    slope_log, r2_log = compute_regression(log_x, y)
    # Quadratic
    x_squared = x ** 2
    slope_quad, r2_quad = compute_regression(x_squared, y)

    name = key.replace("bp", "").capitalize().replace("fit", " Fit")
    print(f"{name} regressions:")
    print(f"  Linear     - Slope: {slope_lin:.2f}")
    print(f"  Logarithmic- Slope: {slope_log:.2f}")
    print(f"  Quadratic  - Slope: {slope_quad:.2f}\n")
