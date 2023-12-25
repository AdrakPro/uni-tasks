import cv2
from mosaic import mosaic_f
from demosaic import demosaic_f
from constants import BAYER, XTRANS

if __name__ == '__main__':
    img = cv2.imread('img/roadrunner.bmp')
    # BAYER or XTRANS
    type = BAYER

    mosaic_f(img, type)
    demosaic_f(img, type)
