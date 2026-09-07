import numpy as np
import matplotlib.pyplot as plt

# function
def f(x):
    return x**2

# interval [a, b]
a = 0
b = 2

# number of subintervals
n = 8

# width of each rectangle
dx = (b - a) / n

# left endpoints
x_left = np.arange(a, b, dx)

# heights of the rectangles
y_left = f(x_left)

# curve points
x_curve = np.linspace(a, b, 400)
y_curve = f(x_curve)

fig, ax = plt.subplots()

# plot the function
ax.plot(x_curve, y_curve)

# draw the rectangles of the Riemann sum
ax.bar(
    x_left,
    y_left,
    width=dx,
    align='edge',
    alpha=0.4,
    edgecolor='black',
)

# some labels
ax.set_xlim(a - 0.1, b + 0.1)
ax.set_ylim(0, f(b) + 0.5)
ax.grid(True)
ax.legend()

riemann_sum = np.sum(y_left * dx)
plt.savefig("/home/pedro/Desktop/pedro-knowledge/Assets/riemannSum.png", dpi=300)
plt.show()