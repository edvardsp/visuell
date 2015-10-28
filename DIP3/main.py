#!/usr/bin/python2

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

Images = ['Fig1043(a)(yeast_USC).tif',
          'Fig1051(a)(defective_weld).tif',
          'noisy.tiff']

##########################################################################
#
#   Main
#
##########################################################################


def main():
    for image in Images:
        img = cv2.imread('images/' + image, 0)
        ts.task1a(img)
        ts.task1b(img)

if __name__ == '__main__':
    main()
