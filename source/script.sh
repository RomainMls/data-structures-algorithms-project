#!/bin/bash

x_values=("next" "worst" "best" "first")
y_values=(100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000 1100000 1200000 1300000 1400000 1500000)

for x in "${x_values[@]}"; do
  for y in "${y_values[@]}"; do
    echo "${x};${y}"
    ./bp${x}fit $y 20
  done
done

