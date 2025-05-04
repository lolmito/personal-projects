import numpy as np
import matplotlib.pyplot as plt

# Membership functions for Hydro input

def hydro_low(x):
    if x <= 0:
        return 1.0
    elif 0 < x <= 2:
        return 1.0 - (x / 2.0)
    elif 2 < x <= 4:
        return (4 - x) / 2.0
    else:
        return 0.0

def hydro_medium(x):
    if x <= 5:
        return 0.0
    elif 5 < x <= 6:
        return (x - 5) / 1.0
    elif 6 < x <= 10:
        return 1.0
    elif 10 < x <= 11:
        return (11 - x) / 1.0
    else:
        return 0.0

def hydro_high(x):
    if x <= 9:
        return 0.0
    elif 9 < x <= 11.5:
        return (x - 9) / 2.5
    elif 11.5 < x <= 12:
        return (x - 11.5) / 0.5
    else:
        return 1.0

# Voltage range (0V to 12V)
x_values = np.linspace(0, 12, 500)

# Calculate membership values
low_values = [hydro_low(x) for x in x_values]
medium_values = [hydro_medium(x) for x in x_values]
high_values = [hydro_high(x) for x in x_values]

# Plotting the membership functions
plt.figure(figsize=(8, 6))
plt.plot(x_values, low_values, label="Low", color='blue')
plt.plot(x_values, medium_values, label="Medium", color='green')
plt.plot(x_values, high_values, label="High", color='red')

plt.title('Fuzzy Membership Functions for Hydro Input')
plt.xlabel('Voltage (V)')
plt.ylabel('Membership Degree')
plt.grid(True)
plt.legend()
plt.show()
