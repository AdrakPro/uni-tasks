import numpy as np


def nearest_neighbour(x, x_nodes, y_nodes):
    """
    :param
    x        - points to interpolate
    x_nodes  - known x nodes
    y_nodes  - known y nodes

    :return
    y        - interpolated points for x
    """

    y = np.zeros_like(x)
    for i, xi in enumerate(x):
        idx = np.argmin(np.abs(x_nodes - xi))
        y[i] = y_nodes[idx]
    return y


def inverse_distance_weighted(x, x_nodes, y_nodes, power=2):
    """
    :param
    x       - points to interpolate
    x_data  - known x nodes
    y_data  - known y nodes

    :return
    y       - interpolated points for x
    """

    y = np.zeros_like(x)

    for i, xi in enumerate(x):
        distances = np.abs(x_nodes - xi)

        # Handle zero distances
        distances[distances == 0] = np.finfo(float).eps

        weights = 1 / (distances ** power)
        normalized_weights = weights / np.sum(weights)
        y[i] = np.sum(normalized_weights * y_nodes)

    return y
