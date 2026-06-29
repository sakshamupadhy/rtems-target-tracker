import matplotlib.pyplot as plt
import csv

time, x, y, z = [], [], [], []

with open('tracking_data.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        try:
            time.append(float(row[0]))
            x.append(float(row[1]))
            y.append(float(row[2]))
        except:
            continue

plt.plot(x, y, label='Drone Trajectory')
plt.title('Target Tracking Path')
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.grid(True)
plt.legend()
plt.show()
