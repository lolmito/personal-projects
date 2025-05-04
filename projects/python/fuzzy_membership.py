import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt

# Define voltage range from 0 to 12V
voltage = np.arange(0, 13, 0.1)

# Define triangular membership functions using trimf
low_voltage = fuzz.trimf(voltage, [0, 0, 12])  # Low: starts at 0V, peaks at 0V, ends at 12V
medium_voltage = fuzz.trimf(voltage, [4, 8, 12])  # Medium: starts at 4V, peaks at 8V, ends at 12V
high_voltage = fuzz.trimf(voltage, [9, 12, 12])  # High: starts at 9V, peaks at 12V, ends at 12V

# Plotting the membership functions
plt.figure(figsize=(8, 6))

plt.plot(voltage, low_voltage, 'b', linewidth=1.5, label='Low Voltage')
plt.plot(voltage, medium_voltage, 'g', linewidth=1.5, label='Medium Voltage')
plt.plot(voltage, high_voltage, 'r', linewidth=1.5, label='High Voltage')

# Labels and title
plt.title('Triangular Membership Functions for Power Source Voltages')
plt.xlabel('Voltage (V)')
plt.ylabel('Membership Degree')
plt.legend()

# Show the plot
plt.grid(True)
plt.show()
