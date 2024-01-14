import warnings

import cv2
import numpy as np

from constants import BAYER, XTRANS


def demosaic_f(img, type):
    mask = []
    filter = []

    if type == BAYER:
        mask = np.ones((2, 2))
        filter = [mask * w for w in [1, 1 / 2, 1]]
    elif type == XTRANS:
        mask = np.array([[0., 0., 0., 0., 0., 0.],
                         [0., 0.25, 0.5, 0.5,
                          0.25,
                          0.],
                         [0., 0.5, 1., 1., 0.5,
                          0.],
                         [0., 0.5, 1., 1., 0.5,
                          0.],
                         [0., 0.25, 0.5, 0.5,
                          0.25,
                          0.],
                         [0., 0., 0., 0., 0.,
                          0.]])
        filter = np.array(
            [mask * i for i in
             [1 / 2, 1 / 5, 1 / 2]])
    else:
        warnings.warn("The type is missing!")

    R, G, B = [
        cv2.filter2D(img[..., i], -1, filter[i])
        for
        i in range(3)]

    rgb = np.dstack((R, G, B))

    cv2.imwrite("./img/demosaic_" + type + ".bmp",
                rgb)

    img -= rgb

    cv2.imwrite("./img/diff_" + type + ".bmp",
                img)
