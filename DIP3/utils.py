from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

# Import libraries
import cv2
import numpy as np

##########################################################################
#
#   Functions
#
##########################################################################


def mean(src, mask=None):
    # Returns the mean of a gray image,
    # optional with mask
    return cv2.mean(src, mask=mask)[0]


def calcThreshold(img):
    # Initial T = mean of img
    T = mean(img)
    # delta T
    dT = 0.1

    # Loop until acceptable T found
    while True:
        # Generate masks split on the given T
        mask1 = np.zeros(img.shape, np.uint8)
        mask2 = np.zeros(img.shape, np.uint8)
        for ind in xrange(img.size):
            if img.item(ind) <= T:
                mask1.itemset(ind, 1)
            else:
                mask2.itemset(ind, 1)

        # Find the mean of the two groups
        m1 = mean(img, mask1)
        m2 = mean(img, mask2)

        # Save old T
        old_T = T
        # Calculate new T
        T = (m1 + m2) / 2.0

        # Check if acceptable
        if abs(T - old_T) < dT:
            break

    # Return threshold found
    return T


def segmentThreshold(img, thresh):
    # Returns a binary gray image with given threshold
    return cv2.threshold(img, thresh, 255, cv2.THRESH_BINARY)[1]


def segmentRegionGrow(img):
    pass
