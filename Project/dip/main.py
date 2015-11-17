#!/usr/bin/python

from __future__ import print_function, division

import sys
import argparse

import cv2
import numpy as np
# import matplotlib.pyplot as plt


def neighborhood(hood, circle):
    def dist(c1, c2):
        c1 = np.int64(c1)
        c2 = np.int64(c2)
        diff = (c1[0] - c2[0])**2 + (c1[1] - c2[1])**2
        return np.sqrt(diff)

    for house in hood:
        center = tuple(house[:2])
        if dist(center, circle) < 60.0:
            return True

    return False


def getCircles(gray):
    gray = cv2.GaussianBlur(gray, (5, 5), 3)

    canny = cv2.Canny(gray, 0, 100)

    circles = cv2.HoughCircles(canny, cv2.HOUGH_GRADIENT, 1, 80,
                               param1=50,
                               param2=15,
                               minRadius=29,
                               maxRadius=35)

    return circles


def addCircles(hood, circles):
    circles = np.uint16(np.around(circles))
    for i in circles[0, :]:
        center = tuple(i[:2])
        if not neighborhood(hood, center):
            hood.append(i)

    return neighborhood


def seperate(gimg, hood):
    white, red = [], []
    definite = np.uint16(np.around(getCircles(gimg)[:, :, :2]))
    for chip in hood:
        center = tuple(chip[:2])
        if neighborhood(definite[0], center):
            white.append(chip)
        else:
            red.append(chip)

    return red, white


def detectCorner(img):
    _, _, r = cv2.split(img)
    h, w = r.shape
    px = (w/16, h/10)
    return 'r' if r[px] > 127 else 'b'


def saveOutput(name, shape, red, white, corner):
    h, w = shape
    if '/' in name:
        file_name = name.partition('/')[-1]
    elif '\\' in name:
        file_name = name.partition('\\')[-1]
    else:
        file_name = name
    output_name = "{0}_output.kappa".format(*file_name.split("."))

    with open(output_name, 'w') as f:
        f.write("{}\n".format(corner))
        for chip in red:
            f.write("{}f,{}f,{}\n".format(chip[0]/w, chip[1]/h, 'r'))
        for chip in white:
            f.write("{}f,{}f,{}\n".format(chip[0]/w, chip[1]/h, 'w'))


# def showOutput(img, red, white):
#     imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

#     for chip in red:
#         center = tuple(chip[:2])
#         cv2.circle(imgRGB, center, chip[2], (0, 255, 255), 2)
#         cv2.circle(imgRGB, center, 2, (0, 0, 255), 3)

#     for chip in white:
#         center = tuple(chip[:2])
#         cv2.circle(imgRGB, center, chip[2], (255, 50, 255), 2)
#         cv2.circle(imgRGB, center, 2, (0, 0, 255), 3)

#     plt.imshow(imgRGB), plt.xticks([]), plt.yticks([])
#     plt.show()


def edgeTest(image):
    img = cv2.imread(image)
    if img is None:
        print('File "{}" does not exist'.format(image))
        sys.exit(1)

    gimg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    total = []
    for ch in cv2.split(img):
        circles = getCircles(ch)

        if circles is not None:
            addCircles(total, circles)

    red, white = seperate(gimg, total)
    corner = detectCorner(img)
    saveOutput(image, gimg.shape, red, white, corner)

#    showOutput(img, red, white)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--image", help="Image to process")

    args = parser.parse_args()

    if args.image is not None:
        edgeTest(args.image)
    else:
        parser.print_usage()


if __name__ == '__main__':
    main()
