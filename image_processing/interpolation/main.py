import numpy as np
import matplotlib.pyplot as plt
from interpolations import nearest_neighbour, inverse_distance_weighted

if __name__ == '__main__':
    x = np.linspace(0, 2*np.pi, 1000)
    y = np.sin(x)

    x_nodes = np.linspace(0, 2*np.pi, 5)
    y_nodes = np.sin(x_nodes)

    # y_interpolated = nearest_neighbour(x, x_nodes, y_nodes)
    y_interpolated = inverse_distance_weighted(x, x_nodes, y_nodes)

    plt.plot(x, y, label="sin(x)", alpha=1)
    plt.scatter(x_nodes, y_nodes, color="red", label="Nodes")
    plt.plot(x, y_interpolated, label="Interpolation", linestyle="--")
    plt.xlabel("x")
    plt.ylabel("sin(x)")
    plt.grid(True)
    plt.legend()
    plt.show()
