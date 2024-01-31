import cv2
import numpy as np


def calculate_errors(img: np.ndarray, img_diff: np.ndarray) -> list:
    """
    Calculate Mean Squared Error (MSE) and Mean Absolute Error (MAE) between two images.

    :param img: (numpy.ndarray) Original image.
    :param img_diff: (numpy.ndarray) Difference image.
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


def convolve(img: np.ndarray, size_of_kernel: tuple[int, int]) -> None:
    """
    Apply convolution filter based on Keys function to the image.

    :param img: (numpy.ndarray) Input image.
    :param size_of_kernel: (int) Size of the convolution kernel.
    """
    kernel = np.ones(size_of_kernel)
    height, width, dim = img.shape
    kernel_size = np.size(kernel[0])
    padded = cv2.copyMakeBorder(
        img,
        kernel_size // 2,
        kernel_size // 2,
        kernel_size // 2,
        kernel_size // 2,
        cv2.BORDER_REPLICATE,
    )
    result = np.zeros((height, width, dim), dtype=np.uint8)
    for x in range(height):
        for y in range(width):
            area = padded[x : x + kernel_size, y : y + kernel_size, :]
            for i in range(3):
                result[x, y, i] = np.sum(area[:, :, i] * kernel) / np.sum(kernel)

    cv2.imwrite("noise/result_conv.jpg", result)
    diff = img - result
    calc_errors = calculate_errors(img, diff)
    print(f"MSE: {calc_errors[0]}, MAE: {calc_errors[1]}")
    cv2.imwrite("noise/diff_conv.jpg", diff)


def median_filter(img: np.ndarray, size_of_kernel: tuple[int, int]) -> None:
    """
    Apply median filter to the image.

    :param img: (numpy.ndarray) Input image.
    :param size_of_kernel: (tuple[int, int]) Size of the median filter kernel.
    """
    height, width, dim = img.shape
    kernel = np.zeros(size_of_kernel)
    kernel_size = np.size(kernel[0])
    padded = cv2.copyMakeBorder(
        img,
        kernel_size // 2,
        kernel_size // 2,
        kernel_size // 2,
        kernel_size // 2,
        cv2.BORDER_REPLICATE,
    )

    result = np.zeros((height, width, dim), dtype=np.uint8)

    for x in range(height):
        for y in range(width):
            area = padded[x : x + kernel_size, y : y + kernel_size, :]
            result[x, y, 0] = np.median(area[:, :, 0])
            result[x, y, 1] = np.median(area[:, :, 1])
            result[x, y, 2] = np.median(area[:, :, 2])

    cv2.imwrite("noise/result_median.jpg", result)
    diff = img - result
    calc_errors = calculate_errors(img, diff)
    print(f"MSE: {calc_errors[0]}, MAE: {calc_errors[1]}")
    cv2.imwrite("noise/diff_median.jpg", diff)


def bilateral_filter(img: np.ndarray, kernel: int) -> None:
    """
    Apply bilateral filter to the image.

    :param img: (numpy.ndarray) Input image.
    :param kernel: (int) Size of the bilateral filter kernel.
    """
    result = cv2.bilateralFilter(img, kernel, 75, 75)
    cv2.imwrite("noise/result_bilateral.jpg", result)
    diff = img - result
    calc_errors = calculate_errors(img, diff)
    print(f"MSE: {calc_errors[0]}, MAE: {calc_errors[1]}")
    cv2.imwrite("noise/diff_bilateral.jpg", diff)


if __name__ == "__main__":
    original_img = cv2.imread("noise/Leopard-with-noise.jpg")

    # MSE: 330.6665983200073, MAE: 308.14006900787354
    convolve(original_img, (5, 5))

    # MSE: 326.53472423553467, MAE: 306.42272090911865
    median_filter(original_img, (25, 25))

    # MSE: 330.0035181045532, MAE: 308.8178548812866
    bilateral_filter(original_img, 25)
