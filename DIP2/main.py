#!/usr/bin/python

from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

import cv2

import utils as us
import tasks as ts

##########################################################################
#
#   Globals
#
##########################################################################

Images = ['4.2.06-lake.tiff', 'bricks.tiff', 'bush.tiff', 'clinton.tiff']

##########################################################################
#
#   Main
#
##########################################################################


def main():
    for image in Images:
        img = cv2.imread('images/' + image, 0)
        optimg = us.makeOptimalSize(img)

        ts.task1a(optimg)
        ts.task1b(optimg)
        ts.task2a(img)
        ts.task2b(img)

    img1 = cv2.imread('images/bush.tiff', 0)
    img2 = cv2.imread('images/clinton.tiff', 0)
    ts.task1c(img1, img2)


if __name__ == '__main__':
    main()
