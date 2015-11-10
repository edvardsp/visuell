#!/usr/bin/python

from __future__ import print_function

import cv2
import numpy as np
import matplotlib.pyplot as plt

from scipy.ndimage import label 

Images = ['image1.png', 'image2.png', 'image3.png']

def auto_canny(img, sigma=0.33):
    v = np.median(img)

    lower = int(max(0, (1.0 - sigma) * v))
    upper = int(min(255, (1.0 + sigma) * v))
    edged = cv2.Canny(img, lower, upper)

    return edged


def edgeTest(img):
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    for ch in list(cv2.split(img)) + [gray]:
        blur = cv2.GaussianBlur(ch, (5, 5), 0)
        method = cv2.THRESH_BINARY + cv2.THRESH_OTSU
        ret, otsu = cv2.threshold(blur, 0, 255, method)
        print('Threshold is {}'.format(ret))

        print(ch[(350, 241)])
        print(ch[(382, 284)])
        cv2.circle(ch, (350, 241), 2, 255, 2)
        cv2.circle(ch, (382, 284), 2, 255, 2)

        plt.subplot(121), plt.imshow(ch, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.subplot(122), plt.imshow(otsu, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.show()

        circles = cv2.HoughCircles(otsu, cv2.HOUGH_GRADIENT, 1, 100,
                                   param1=50,
                                   param2=15,
                                   minRadius=25,
                                   maxRadius=35)

        if circles is not None:
            print(circles)
            circles = np.uint16(np.around(circles))
            for i in circles[0, :]:
                cv2.circle(imgRGB, (i[0], i[1]), i[2], (0, 255, 0), 2)
                cv2.circle(imgRGB, (i[0], i[1]), 2, (0, 0, 255), 3)

    plt.imshow(imgRGB), plt.xticks([]), plt.yticks([])
    plt.show()


def edgeTest2(img):
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    gimg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    for ch in list(cv2.split(img)) + [gimg]:
        gray = cv2.GaussianBlur(ch, (5, 5), 3)

        border = cv2.dilate(gray, None, iterations=5)
        border = border - cv2.erode(border, None)

        edges = gray - cv2.erode(gray, None)
        ret, bin_edge = cv2.threshold(edges, 0, 255, cv2.THRESH_OTSU)
        fill = bin_edge.copy()
        h, w = fill.shape
        mask = np.zeros((h + 2, w + 2), dtype=np.uint8)

        circles = cv2.HoughCircles(bin_edge, cv2.HOUGH_GRADIENT, 1, 80,
                                   param1=50,
                                   param2=15,
                                   minRadius=29,
                                   maxRadius=35)

        if circles is not None:
            print(circles)
            circles = np.uint16(np.around(circles))
            for i in circles[0, :]:
                center = tuple(i[:2])
                cv2.circle(imgRGB, center, i[2], (0, 255, 0), 2)
                cv2.circle(imgRGB, center, 2, (0, 0, 255), 3)
                cv2.floodFill(fill, mask, center, 255)

        plt.subplot(221), plt.imshow(gray, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.subplot(222), plt.imshow(edges, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.subplot(223), plt.imshow(bin_edge, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.subplot(224), plt.imshow(fill, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.show()

    plt.imshow(imgRGB), plt.xticks([]), plt.yticks([])
    plt.show()


def edgeTest3(img):
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    gimg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    for ch in list(cv2.split(img)) + [gimg]:
        gray = cv2.GaussianBlur(ch, (5, 5), 3)

        border = gray - cv2.erode(gray, None)

        lbl = np.zeros(gray.shape, np.int32)

        h, w = gray.shape
        markers = [(h/5*j+h/10, w/8*i+w/16) for i in range(8) for j in range(5)]
        mask = np.zeros((h+2, w+2), dtype=np.uint8)
        for mark in markers:
            lbl[mark] = gray[mark]
            cv2.floodFill(border, mask, mark[::-1], 255)

        cv2.watershed(img, lbl)
        lbl[lbl < 1] = 0

        lbl = np.uint8(lbl)
        lbl = cv2.erode(lbl, None)
        lbl[lbl != 0] = 255

        plt.subplot(121), plt.imshow(ch, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.subplot(122), plt.imshow(lbl, cmap='gray'), plt.xticks([]), plt.yticks([])
        plt.show()


def plot(imgs, titles):
    i = 1
    for img, title in zip(imgs, titles):
        if i == 1:
            plt.subplot(2, 2, i), plt.imshow(img)
        else:
            plt.subplot(2, 2, i), plt.imshow(img, cmap='gray')
        plt.title(title), plt.xticks([]), plt.yticks([])
        i += 1

    plt.show()


def main():
    for image in Images:
        print('Image "{}"'.format(image))
        img = cv2.imread('images/' + image)
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        b, g, r = cv2.split(img)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        #plot([imgRGB, b, g, r], ['Original', 'Blue ch', 'Green Ch', 'Red ch'])

        #edgeTest(img)
        #edgeTest2(img)
        edgeTest3(img)


if __name__ == '__main__':
    main()
