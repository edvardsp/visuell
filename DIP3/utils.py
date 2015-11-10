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
    """
    Returns the mean of a gray image,
    optional with mask
    """
    return cv2.mean(src, mask=mask)[0]


def calcThreshold(img):
    """
    Calculates a good estimate of the threshold value for a given image.
    """
    # Initial T = mean of img
    T = mean(img)
    # delta T
    dT = 0.1

    # Loop until acceptable T found
    while True:
        # Generate masks split on the given T
        mask1 = np.uint8(np.where(img < int(T), 1, 0))
        mask2 = np.uint8(np.where(img > int(T), 1, 0))

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
    """
    Returns a binary gray image with given threshold.
    """
    return cv2.threshold(img, thresh, 255, cv2.THRESH_BINARY)[1]


def segmentRegionGrow(img, seeds, thresh, hood='moore'):
    """
    Segments an image with region growing, given with a list of seeds,
    and a difference threshold to seperate the regions.
    Can choose between a Moore or von Neumann neighborhood.
    """
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
    """
    Removes noise from a given image through morphology.
    Result image is given as a binary image.
    """
    # Turn image into binary image
    bin_img = segmentThreshold(img, 255/2)

    # Kernel used in morphology
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (6, 6))

    # Morphology OPEN, which removes noise outside the object
    opening = cv2.morphologyEx(bin_img, cv2.MORPH_OPEN, kernel, iterations=3)
    # Morphology CLOSE, which removes noise within the object
    result = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel, iterations=2)

    # Return the result
    return result


def morphDistTransform(img):
    """
    Computes and returns a chess distance transform of a given image
    with the use of morphology.
    """
    # Get the noise free image
    noisefree = morphNoiseRemoval(img)
    # Kernel to be used in morphology
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))

    # Variable to be eroded on each iteration
    aux_img = noisefree

    # Keeps track of levels
    levels = np.empty(img.shape, dtype=np.uint8)
    levels.fill(255)
    mask = np.zeros(img.shape, dtype=np.uint8)

    # Current iterations of erosion
    curr_level = -1

    # While there are pixels not eroded away
    ones = np.ones(img.shape, dtype=np.uint8)
    while not (mask == ones).all():
        # Update the iteration
        curr_level += 1

        # Create mask of eroded pixels
        mask = np.uint8(np.where(aux_img == 0, 1, 0))
        # Subtract from levels
        levels -= mask

        # Erode the image on more iteration
        aux_img = cv2.morphologyEx(aux_img, cv2.MORPH_ERODE, kernel)

    # Scale the result to the range(0, 256)
    A = 255 / curr_level
    B = 255 * levels.min() / curr_level
    levels = A * levels - B

    # Return the result
    return np.uint8(levels)


def morphExtractBound(img):
    """
    Computes and returns a boundary image of a given image with
    the use of morphology.
    """
    # Kernel to be used in morphology
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))

    # Get the noise free image
    noisefree = morphNoiseRemoval(img)
    # Erosion of the noise free image for k iterations
    erode = cv2.morphologyEx(noisefree, cv2.MORPH_ERODE, kernel, iterations=3)

    # Return the boundary image
    return noisefree & ~erode
