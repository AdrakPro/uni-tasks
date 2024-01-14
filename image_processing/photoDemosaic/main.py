import cv2

from constants import XTRANS
from demosaic import demosaic_f
from mosaic import mosaic_f

if __name__ == '__main__':
    img = cv2.imread('img/roadrunner.bmp')
    # BAYER or XTRANS
    type = XTRANS

    mosaic_f(img, type)
    demosaic_f(img, type)
