from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

# Import libraries
import cv2
import numpy as np
import queue as qu

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
        for ind in range(img.size):
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


def segmentRegionGrow(img, seeds, thresh, hood='moore'):
    # Parameters
    h, w = img.shape
    output = np.zeros(img.shape, dtype=np.uint8)

    # Variables
    if hood == 'moore':
        dirs = tuple(
            (x, y)
            for x in range(-1, 2)
            for y in range(-1, 2)
            if x != 0 or y != 0)
    elif hood == 'neumann':
        dirs = tuple(
            (x, y)
            for x in range(-1, 2)
            for y in range(-1, 2)
            if x != 0 ^ y != 0)
    else:
        raise TypeError('Hood not valid: %s' % hood)

    visited = {(x, y): False for x in range(w) for y in range(h)}
    in_queue = {(x, y): False for x in range(w) for y in range(h)}

    # Aux functions
    def get_val(px):
        ind = px[1] * w + px[0]
        return img.item(ind)

    def set_val(px, val):
        ind = px[1] * w + px[0]
        output.itemset(ind, val)

    def expand(px):
        return (tuple(map(sum, zip(px, d))) for d in dirs)

    def in_img(px):
        return 0 <= px[0] < w and 0 <= px[1] < h

    # For all seeds
    for seed in seeds:

        # Initialize queue
        queue = qu.Queue()
        queue.put(seed)
        in_queue[seed] = True

        # Seed value to follow
        mean_val = get_val(seed)

        # Expand
        while not queue.empty():
            # Get current pixel
            curr_px = queue.get()
            curr_val = get_val(curr_px)
            visited[curr_px] = True

            # Add to region?
            diff = abs(mean_val - curr_val)
            if diff < thresh:
                # Set it white
                set_val(curr_px, 255)

                # Add valid neighboring pixels
                for px in filter(lambda px: in_img(px), expand(curr_px)):
                    if not visited[px] and not in_queue[px]:
                        queue.put(px)
                        in_queue[px] = True

            # Else remove from visited, so other regions can access it
            else:
                visited[px] = False
                in_queue[px] = False

    # Return the result
    return output


def morphNoiseRemoval(img):
    bin_img = segmentThreshold(img, 255/2)

    kernel1 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (6, 6))
    kernel2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2, 2))

    opening = cv2.morphologyEx(bin_img, cv2.MORPH_OPEN, kernel1, iterations=3)
    closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel1, iterations=2)

    result = cv2.morphologyEx(closing, cv2.MORPH_OPEN, kernel2, iterations=5)

    return result









