#!/usr/bin/python

from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

# Import libraries
import cv2

# Import local files
import tasks as ts

##########################################################################
#
#   Globals
#
##########################################################################

Seg_Imgs = ['Fig1043(a)(yeast_USC).tif',
            'Fig1051(a)(defective_weld).tif']
Seg_Seeds = [[(78, 255), (179, 514), (157, 251), (255, 483),
              (306, 267), (442, 397), (465, 325), (428, 265),
              (369, 225), (419, 169), (568, 205), (393, 88),
              (356, 517), (449, 580), (500, 642), (573, 698)],
             [(140, 254), (295, 254), (414, 233), (440, 238)]]
Segmentation = zip(Seg_Imgs, Seg_Seeds)

Morph_Imgs = ['noisy.tiff']

##########################################################################
#
#   Main
#
##########################################################################


def main():
    for image, seeds in Segmentation:
        img = cv2.imread('images/' + image, 0)

        ts.task1a(img)
        ts.task1b(img, seeds)

    for image in Morph_Imgs:
        img = cv2.imread('images/' + image, 0)

        ts.task2a(img)
        ts.task2b(img)
        ts.task2c(img)


if __name__ == '__main__':
    main()
