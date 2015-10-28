from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

# Import libraries
import cv2
import numpy as np
from matplotlib import pyplot as plt

# Import local files
import utils as us

##########################################################################
#
#   Functions
#
##########################################################################


def setWindowMax():
    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()


def showImages(imgs, titles):
    diff = len(imgs) - len(titles)
    if diff > 0:
        titles += [''] * diff

    for i, img in enumerate(imgs):
        plt.subplot(1, len(imgs), i+1), plt.imshow(img, cmap='gray')
        plt.title(titles[i]), plt.xticks([]), plt.yticks([])

    setWindowMax()
    plt.show()

##########################################################################
#
#   Tasks
#
##########################################################################


def task1a(img):
    print('Task 1A')

    T = us.calcThreshold(img)
    thresh = us.segmentThreshold(img, T)

    showImages([img, thresh], ['Original', 'Segmented T = %i' % T])

    print('Done')


def task1b(img):
    print('Task 1B')

    us.segmentRegionGrow(img)

    print('Done')
