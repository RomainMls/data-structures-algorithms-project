import re
import numpy as np
from sklearn.linear_model import LinearRegression

# Load file
with open("stats.txt", "r") as f:
    lines = f.readlines()

# Initialize containers
data = {
    "bpnextfit": {"x": [], "lost": []},
    "bpworstfit": {"x": [], "lost": []},
    "bpbestfit": {"x": [], "lost": []},
    "bpfirstfit": {"x": [], "lost": []}
}

current_algo = ""
for line in lines:
    line = line.strip()
    if line.startswith("./") or line.startswith("bp"):
        current_algo = line.split()[0].replace("./", "")
        if current_algo in data:
            data[current_algo]["x"].append(int(line.split()[1]))
    elif "avg lost" in line and current_algo in data:
        lost = int(re.search(r"avg lost = (\d+)", line).group(1))
        data[current_algo]["lost"].append(lost)

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
    print(f"  Linear     - Slope: {slope_lin:.2f}, R²: {r2_lin:.3f}")
    print(f"  Logarithmic- Slope: {slope_log:.2f}, R²: {r2_log:.3f}")
    print(f"  Quadratic  - Slope: {slope_quad:.2f}, R²: {r2_quad:.3f}\n")
