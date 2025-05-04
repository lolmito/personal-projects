import numpy as np
import matplotlib.pyplot as plt

# Membership functions for SOC
def soc_low(soc):
    return max(0, min(1, (30 - soc) / 30)) if soc <= 30 else 0

def soc_medium(soc):
    return max(0, min((soc - 30) / 40, (70 - soc) / 30)) if 30 <= soc <= 70 else 0

def soc_high(soc):
    return max(0, min(1, (soc - 70) / 30)) if soc >= 70 else 0

# Membership functions for Solar Voltage (0V to 12V)
def solar_low(voltage):
    return max(0, min(1, (6 - voltage) / 6)) if voltage <= 6 else 0

def solar_medium(voltage):
    return max(0, min((voltage - 5) / 3, (10 - voltage) / 3)) if 5 <= voltage <= 10 else 0

def solar_high(voltage):
    return max(0, min(1, (voltage - 9) / 3)) if voltage >= 9 else 0


# Membership functions for Hydro Voltage (0V to 12V)
def hydro_low(voltage):
    return solar_low(voltage)

def hydro_medium(voltage):
    return solar_medium(voltage)

def hydro_high(voltage):
    return solar_high(voltage)

# Membership functions for Grid Voltage (0V to 12V)
def grid_low(voltage):
    return solar_low(voltage)

def grid_medium(voltage):
    return solar_medium(voltage)

def grid_high(voltage):
    return solar_high(voltage)

# Generate values
soc_values = np.linspace(0, 100, 100)
solar_voltage = np.linspace(0, 12, 100)
hydro_voltage = np.linspace(0, 12, 100)
grid_voltage = np.linspace(0, 12, 100)

# Calculate membership values
soc_low_values = [soc_low(x) for x in soc_values]
soc_medium_values = [soc_medium(x) for x in soc_values]
soc_high_values = [soc_high(x) for x in soc_values]

solar_low_values = [solar_low(x) for x in solar_voltage]
solar_medium_values = [solar_medium(x) for x in solar_voltage]
solar_high_values = [solar_high(x) for x in solar_voltage]

hydro_low_values = [hydro_low(x) for x in hydro_voltage]
hydro_medium_values = [hydro_medium(x) for x in hydro_voltage]
hydro_high_values = [hydro_high(x) for x in hydro_voltage]

grid_low_values = [grid_low(x) for x in grid_voltage]
grid_medium_values = [grid_medium(x) for x in grid_voltage]
grid_high_values = [grid_high(x) for x in grid_voltage]

# Plotting
fig, axs = plt.subplots(4, 1, figsize=(10, 15))

# SOC Membership Functions
axs[0].plot(soc_values, soc_low_values, label='Low SOC', color='red')
axs[0].plot(soc_values, soc_medium_values, label='Medium SOC', color='orange')
axs[0].plot(soc_values, soc_high_values, label='High SOC', color='green')
axs[0].set_title('State of Charge (SOC) Membership Functions')
axs[0].set_xlabel('SOC (%)')
axs[0].set_ylabel('Membership Degree')
axs[0].legend()
axs[0].grid()

# Solar Voltage Membership Functions
axs[1].plot(solar_voltage, solar_low_values, label='Low Solar', color='red')
axs[1].plot(solar_voltage, solar_medium_values, label='Medium Solar', color='orange')
axs[1].plot(solar_voltage, solar_high_values, label='High Solar', color='green')
axs[1].set_title('Solar Voltage Membership Functions')
axs[1].set_xlabel('Voltage (V)')
axs[1].set_ylabel('Membership Degree')
axs[1].legend()
axs[1].grid()

# Hydro Voltage Membership Functions
axs[2].plot(hydro_voltage, hydro_low_values, label='Low Hydro', color='red')
axs[2].plot(hydro_voltage, hydro_medium_values, label='Medium Hydro', color='orange')
axs[2].plot(hydro_voltage, hydro_high_values, label='High Hydro', color='green')
axs[2].set_title('Hydro Voltage Membership Functions')
axs[2].set_xlabel('Voltage (V)')
axs[2].set_ylabel('Membership Degree')
axs[2].legend()
axs[2].grid()

# Grid Voltage Membership Functions
axs[3].plot(grid_voltage, grid_low_values, label='Low Grid', color='red')
axs[3].plot(grid_voltage, grid_medium_values, label='Medium Grid', color='orange')
axs[3].plot(grid_voltage, grid_high_values, label='High Grid', color='green')
axs[3].set_title('Grid Voltage Membership Functions')
axs[3].set_xlabel('Voltage (V)')
axs[3].set_ylabel('Membership Degree')
axs[3].legend()
axs[3].grid()

plt.tight_layout(pad=5.0)
plt.show()
