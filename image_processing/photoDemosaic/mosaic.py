import warnings

import cv2

from constants import BAYER, XTRANS


def mosaic_f(img, type):
    img_mosaic = []

    if type == BAYER:
        img_mosaic = bayer_mosaic(img)
    elif type == XTRANS:
        img_mosaic = xtrans_mosaic(img)
    else:
        warnings.warn(
            "The type is missing")
    cv2.imwrite(
        "./img/mosaic_" + type + ".bmp",
        img_mosaic)


def bayer_mosaic(img):
    height, width = img.shape[:2]

    for i in range(height):
        for j in range(width):
            if (
                    i % 2 == 0 and j % 2 == 0) or (
                    i % 2 == 1 and j % 2 == 1):  # green
                img[i][j] = [0,
                             img[i][j][
                                 1], 0]
            elif i % 2 == 0 and j % 2 == 1:  # blue
                img[i][j] = [0, 0,
                             img[i][j][
                                 2]]
            elif i % 2 == 1 and j % 2 == 0:  # red
                img[i][j] = [
                    img[i][j][0], 0, 0]
    return img


def xtrans_mosaic(img):
    height, width = img.shape[:2]

    for i in range(height):
        for j in range(width):
            if ((
                        i % 6 == 0 or i % 6 == 3) and (
                        j % 6 == 0 or j % 6 == 3)) or (
                    (
                            i % 6 == 1 or i % 6 == 2 or i % 6 == 4 or i % 6 == 5) and (
                            j % 6 == 1 or j % 6 == 2 or j % 6 == 4 or j % 6 == 5)):  # green
                img[i][j] = [0, img[i][j][1], 0]
            elif (i % 6 == 0 and (
                    j % 6 == 2 or j % 6 == 4)) or (
                    (
                            i % 6 == 1 or i % 6 == 5) and j % 6 == 0) or (
                    (
                            i % 6 == 2 or i % 6 == 4) and j % 6 == 3) or (
                    i % 6 == 3 and (
                    j % 6 == 1 or j % 6 == 5)):  # blue
                img[i][j] = [0, 0,
                             img[i][j][
                                 2]]
            elif ((
                          i % 6 == 1 or i % 6 == 5) and j % 6 == 3) or (
                    (
                            i % 6 == 2 or i % 6 == 4) and j % 6 == 0) or (
                    i % 6 == 3 and (
                    j % 6 == 2 or j % 6 == 4)) or (
                    i % 6 == 0 and (
                    j % 6 == 1 or j % 6 == 5)):  # red
                img[i][j] = [
                    img[i][j][0], 0, 0]
    return img
