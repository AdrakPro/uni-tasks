import cv2

from constants import XTRANS, BAYER
from demosaic import demosaic_f
from mosaic import mosaic_f

if __name__ == "__main__":
    original_img = cv2.imread("img/roadrunner.bmp")
    # BAYER or XTRANS
    filter_type = BAYER

    mosaic_f(original_img, filter_type)
    demosaic_f(original_img, filter_type)
