import warnings

import cv2
import numpy as np

from constants import BAYER, XTRANS


def demosaic_f(img: np.ndarray, filter_type: str) -> None:
    """
    Demosaic an image using a specified color filter array pattern.

    :param img: (numpy.ndarray) Input image.
    :param filter_type: (str) Color filter array type. Should be either 'BAYER' or 'XTRANS'.
    """
    color_filter = []

    if filter_type == BAYER:
        mask = np.ones((2, 2))
        color_filter = [mask * w for w in [1, 1 / 2, 1]]
    elif filter_type == XTRANS:
        mask = np.array(
            [
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [0.0, 0.25, 0.5, 0.5, 0.25, 0.0],
                [0.0, 0.5, 1.0, 1.0, 0.5, 0.0],
                [0.0, 0.5, 1.0, 1.0, 0.5, 0.0],
                [0.0, 0.25, 0.5, 0.5, 0.25, 0.0],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
            ]
        )
        color_filter = np.array([mask * i for i in [1 / 2, 1 / 5, 1 / 2]])
    else:
        warnings.warn("The type is missing!")

    R, G, B = [cv2.filter2D(img[..., i], -1, color_filter[i]) for i in range(3)]
    rgb = np.dstack((R, G, B))
    cv2.imwrite("./img/demosaic_" + filter_type + ".bmp", rgb)

    img -= rgb
    cv2.imwrite("./img/diff_" + filter_type + ".bmp", img)
