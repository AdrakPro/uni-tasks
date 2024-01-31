import math
import time
from typing import Callable

import cv2
import numpy as np


def rectangle_function(x: float) -> float:
    """
    Implements rectangle function for interpolation.

    :param x: (float) Input parameter for the function.
    :return: (float) Returns 1 if -0.5 <= x < 0.5, and 0 otherwise.
    """
    if -0.5 <= x < 0.5:
        return 1.0
    else:
        return 0.0


def triangle_function(x: float) -> float:
    """
     Implements triangle function for interpolation.

    :param x: (float) Input parameter for the function.
    :return: (float) Returns 1 - abs(x) if abs(x) < 1, and 0 otherwise.
    """
    if abs(x) < 1:
        return 1 - abs(x)
    else:
        return 0


def keys_function(x: float) -> float:
    """
     Implements Keys function for interpolation.

    :param x: (float) Input parameter for the function.
    :return: (float) Interpolated value based on specific conditions.
    """
    alpha = -0.5
    x = abs(x)
    if 0 <= x < 1:
        return (alpha + 2) * x**3 - (alpha + 3) * x**2 + 1
    elif 1 <= x < 2:
        return alpha * x**3 - 5 * alpha * x**2 + 8 * alpha * x - 4 * alpha
    return 0


def calculate_errors(img: np.ndarray, img_diff: np.ndarray) -> list:
    """
    Calculates Mean Squared Error (MSE) and Mean Absolute Error (MAE) between two images.

    :param img: (numpy.ndarray) Original image.
    :param img_diff: (numpy.ndarray) Image to compare against the original.
    :return: (list) List containing MSE and MAE.
    """
    height, width = img.shape[:2]
    mse = (img - img_diff) ** 2
    mae = abs(img - img_diff)
    err1, err2 = 0, 0
    for h in range(0, height):
        for w in range(0, width):
            err1 = err1 + mse[h][w][0] + mse[h][w][1] + mse[h][w][2]
            err2 = err2 + mae[h][w][0] + mae[h][w][1] + mae[h][w][2]

    return [err1 / (width * height), err2 / (width * height)]


def scale_img(
    img: np.ndarray, ratio: float, func: Callable[[float], float]
) -> np.ndarray:
    """
    Scales an input image using a specified scaling ratio and interpolation function.

    :param img: (numpy.ndarray) Input image.
    :param ratio: (float) Scaling ratio.
    :param func: (function) Interpolation function.
    :return: (numpy.ndarray) Scaled image.
    """
    img = img.astype(np.float64) / 255

    old_height, old_width = img.shape[:2]
    new_height = math.floor((old_height - 4) * ratio)
    new_width = math.floor((old_width - 4) * ratio)

    new_image = np.zeros((new_height, new_width))

    for j in range(new_height):
        y = ((j + 0.5) / ratio) - 0.5 + 2
        iy = int(y)
        decy = iy - y
        for i in range(new_width):
            x = ((i + 0.5) / ratio) - 0.5 + 2
            ix = int(x)
            decx = ix - x

            pixel = sum(
                img[iy + M, ix + L] * func(decx + L) * func(decy + M)
                for L in range(-1, 2 + 1)
                for M in range(-1, 2 + 1)
            )

            new_image[j, i] = pixel

    return (new_image * 255).round().astype(np.uint8)


def clip(n: int, nmax: int, nmin: int = 0) -> int:
    """
    Clips a value within a specified range.

    :param n: (int) Value to be clipped.
    :param nmax: (int) Maximum value for clipping.
    :param nmin: (int, optional) Minimum value for clipping. Defaults to 0.
    :return: (int) Clipped value.
    """
    if n < 0:
        return nmin
    elif n < nmax:
        return n
    else:
        return nmax - 1


def range_clip(n: int, m: int, nmax: int) -> tuple:
    """
    Clips two values within a specified range.

    :param n: (int) First value to be clipped.
    :param m: (int) Second value to be clipped.
    :param nmax: (int) Maximum value for clipping.
    :return: (tuple) Clipped values.
    """
    return clip(n, nmax), clip(m, nmax)


def interpolate(
    img: np.ndarray, x: float, y: float, func: Callable[[float], float]
) -> float:
    """
    Interpolates values in an image using a specified function.

    :param img: (numpy.ndarray) Image to interpolate.
    :param x: (float) X-coordinate for interpolation.
    :param y: (float) Y-coordinate for interpolation.
    :param func: (function, optional) Interpolation function. Defaults to keys_function.
    :return: (float) Interpolated value.
    """
    height, width = img.shape[:2]
    delta = 3
    ix, iy = int(x), int(y)

    f = 0.0
    for n in range(*range_clip(ix - delta, ix + delta, height)):
        for m in range(*range_clip(iy - delta, iy + delta, width)):
            f += func(x - n) * func(y - m) * img[n, m]
    return f


def rotate_image(
    img: np.ndarray, angle: float, func: Callable[[float], float]
) -> np.ndarray:
    """
    Rotates an image by a specified angle using a given interpolation function.

    :param img: (numpy.ndarray) Input image.
    :param angle: (float) Rotation angle in degrees.
    :param func: (function) Interpolation function.
    :return: (numpy.ndarray) Rotated image.
    """
    radians = np.radians(angle)
    m = len(img)
    size = img.shape[0]

    OXY = np.array([m / 2, m / 2])
    rotation_matrix = np.array(
        [[np.cos(radians), -np.sin(radians)], [np.sin(radians), np.cos(radians)]]
    )

    out = [
        [
            interpolate(
                img,
                *(OXY + rotation_matrix @ (np.array([n / size, m / size]) * m - OXY)),
                func,
            )
            for m in range(size)
        ]
        for n in range(size)
    ]
    return np.array(out)


if __name__ == "__main__":
    original_img = cv2.imread("img/img.bmp")

    start = time.perf_counter()

    height, width, c = original_img.shape

    channels = cv2.split(original_img)

    # Change interpolation function
    interpolation = rectangle_function

    print(
        f"Scaling image from {width}x{height} to {int(width * 0.5)}x{int(height * 0.5)}..."
    )

    scaled_down = cv2.merge(
        list(scale_img(channels[c], 0.5, interpolation) for c in range(c))
    )
    print(
        f"Scaling image from {width}x{height} to {int(width * 2)}x{int(height * 2)}..."
    )

    c2 = scaled_down.shape[2]
    channels2 = cv2.split(scaled_down)
    scaled_up = cv2.merge(
        list(scale_img(channels2[c], 2, interpolation) for c in range(c2))
    )

    rotated_img = rotate_image(original_img, 90, interpolation)

    print(f"Finished in {time.perf_counter() - start} seconds")
    cv2.imwrite("img/rotated.bmp", rotated_img)
    # Rotate returns array with different dimensions, primitive fix
    diff = original_img - np.pad(scaled_up, ((0, 12), (0, 12), (0, 0)), mode="constant")
    cv2.imwrite("diff.bmp", diff)
    mse, mae = calculate_errors(original_img, diff)
    print(f"Mean Squared Error: {mse}")
    print(f"Mean Absolute Error: {mae}")

    # Keys
    # Finished in 56 seconds
    # MSE: 98.67
    # MAE: 130.43

    # Function B
    # Finished in 46.39 seconds
    # MSE: 99.80
    # MAE: 130.38

    # Function a
    # Finished in 62.224294428000576 seconds
    # MSE: 97.96
    # MAE: 130.04

