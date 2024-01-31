import math
import time

import cv2
import numpy as np


def function_a(x):
    if -0.5 <= x < 0.5:
        return 1
    else:
        return 0


def function_b(x):
    if abs(x) < 1:
        return 1 - abs(x)
    else:
        return 0


def keys_function(x):
    alpha = -0.5
    x = abs(x)
    if 0 <= x < 1:
        return (alpha + 2) * x**3 - (alpha + 3) * x**2 + 1
    elif 1 <= x < 2:
        return alpha * x**3 - 5 * alpha * x**2 + 8 * alpha * x - 4 * alpha
    return 0


# Wzory na błędy podane w instrukcji
def calculate_errors(img, img_diff):
    height, width = img.shape[:2]
    mse = (img - img_diff) ** 2
    mae = abs(img - img_diff)
    err1, err2 = 0, 0
    for h in range(0, height):
        for w in range(0, width):
            err1 = err1 + mse[h][w][0] + mse[h][w][1] + mse[h][w][2]
            err2 = err2 + mae[h][w][0] + mae[h][w][1] + mae[h][w][2]

    return [err1 / (width * height), err2 / (width * height)]


def scale_img(img, ratio: float, func):
    img = img.astype(np.float64) / 255

    old_height, old_width = img.shape[:2]
    new_height = math.floor((old_height - 4) * ratio)
    new_width = math.floor((old_width - 4) * ratio)

    new_image = np.zeros((new_height, new_width))

    for j in range(new_height):
        y = ((j + 0.5) / ratio) - 0.5 + 2
        # iy i ix to frakcje
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


def clip(n, nmax, nmin=0):
    if n < 0:
        return nmin
    elif n < nmax:
        return n
    else:
        return nmax - 1


def range_clip(n, m, nmax):
    return clip(n, nmax), clip(m, nmax)


def interpolate(img, x, y, func=keys_function):
    height, width = img.shape[:2]
    # Zakres iterowania pobliskich pikseli
    delta = 3
    ix, iy = int(x), int(y)

    f = 0.0
    for n in range(*range_clip(ix - delta, ix + delta, height)):
        for m in range(*range_clip(iy - delta, iy + delta, width)):
            f += func(x - n) * func(y - m) * img[n, m]
    return f


def rotate_image(img, angle, func):
    radians = np.radians(angle)
    M = len(img)
    size = img.shape[0]
    out = np.empty((size, size))

    OXY = np.array([M / 2, M / 2])
    rotation_matrix = np.array(
        [[np.cos(radians), -np.sin(radians)], [np.sin(radians), np.cos(radians)]]
    )

    out = [
        [
            interpolate(
                img,
                *(OXY + rotation_matrix @ (np.array([n / size, m / size]) * M - OXY)),
                func,
            )
            for m in range(size)
        ]
        for n in range(size)
    ]
    return np.array(out)


if __name__ == "__main__":
    original_img = cv2.imread("img.bmp")

    start = time.perf_counter()

    height, width, c = original_img.shape

    channels = cv2.split(original_img)

    # Zmiana funkcji interpolujacej
    interpolation = function_a

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

    b = np.pad(scaled_up, ((0, 12), (0, 12), (0, 0)), mode="constant")

    a = rotate_image(original_img, 90, interpolation)

    print(f"Finished in {time.perf_counter() - start} seconds")
    cv2.imwrite("rotated.bmp", a)
    diff = original_img - b
    cv2.imwrite("diff.bmp", diff)
    mse, mae = calculate_errors(original_img, diff)
    print(f"Błąd średniokwadratowy: {mse}")
    print(f"Średni błąd względny: {mae}")

    # Keys
    # Finished in 56 seconds
    # Błąd średniokwadratowy: 98.67
    # Średni błąd względny: 130.43

    # Function B
    # Finished in 46.39 seconds
    # Błąd średniokwadratowy: 99.80
    # Średni błąd względny: 130.38

    # Function a
    # Finished in 62.224294428000576 seconds
    # Błąd średniokwadratowy: 9 7.96
    # Średni błąd względny: 130.04
