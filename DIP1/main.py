from __future__ import print_function

import cv2
import numpy as np
from matplotlib import pyplot as plt

color_txts = ['4.1.07-jelly-beans.tiff', '4.2.06-lake.tiff']
gray_txts = ['5.1.10-aerial.tiff', '5.2.09-aerial.tiff']


def convert_gray_1(img):
    """
    gray_ij = (R_ij + G_ij + B_ij)/3.
    """
    # Create the result array
    out = np.ndarray(img.shape[:2], dtype=np.uint8)

    # For each pixel
    for i in range(img.shape[0] * img.shape[1]):
        # Calculate the greyscale value and set it
        gray = (
            img.item(3 * i) + img.item(3 * i + 1) + img.item(3 * i + 2)) / 3
        out.itemset(i, gray)

    # Return the result
    return out


def convert_gray_2(img):
    """
    gray_ij = 0.2126 R_ij + 0.7152 G_ij + 0.0722 B_ij.
    """
    # Create the result array
    out = np.ndarray(img.shape[:2], dtype=np.uint8)

    # For each pixel
    for i in range(img.shape[0] * img.shape[1]):
        # Calculate the greyscale value and set it
        gray = int(0.2126*img.item(3*i) +
                   0.7152*img.item(3*i+1) +
                   0.0722*img.item(3*i+2))
        out.itemset(i, gray)

    # Return the result
    return out


def gray_transform(gimg, T):
    """
    Transformation of a grayscale image with
    transformation function T.
    """
    # Create the result array
    out = np.ndarray(gimg.shape, dtype=np.uint8)

    # For each pixel
    for i in range(gimg.shape[0] * gimg.shape[1]):
        # Calculate the new greyscale value
        grey = gimg.item(i)
        # Max at 255
        grey = min(T(grey), 255)
        # Set value
        out.itemset(i, grey)

    # Return the result
    return out


def conv(gimg, kernel):
    """
    3x3 2D convolution of a given kernel
    """
    # Padded input image
    pad = cv2.copyMakeBorder(gimg, 1, 1, 1, 1, cv2.BORDER_REPLICATE)
    # Result array, with signed int64
    out = np.ndarray(gimg.shape, dtype=np.int64)

    # Different sizes of the arrays
    gimg_x, gimg_y = gimg.shape
    pad_x, pad_y = pad.shape

    # Helper function for padded array
    def f(ind, i, j):
        return pad.item(ind + (j-1) + (i-1) * pad_y)

    # Helper function for kernel array
    def h(i, j):
        return kernel.item(i*3 + j)

    # For each pixel
    for y in range(1, gimg_y + 1):
        for x in range(1, gimg_x + 1):
            # Compute the convolution value
            val = 0
            ind = pad_y * y + x
            for i in range(0, 3):
                for j in range(0, 3):
                    val += f(ind, i, j) * h(i, j)

            # Set the value
            out.itemset((y-1) * gimg_y + (x-1), int(val))

    # Return the result
    return out


def task1():
    print('Task 1')

    # For each image file
    for img_txt in color_txts:
        # Read image
        img = cv2.imread('images/' + img_txt)

        # Calculate each gray scale
        out1 = convert_gray_1(img)
        out2 = convert_gray_2(img)

        # Subplot them
        plt.subplot(131), plt.imshow(img), plt.title('Original')
        plt.subplot(132), plt.imshow(out1, 'gray'), plt.title('Gray 1')
        plt.subplot(133), plt.imshow(out2, 'gray'), plt.title('Gray 2')

        # Resize window
        mngr = plt.get_current_fig_manager()
        mngr.window.setGeometry(50, 100, 1200, 545)

        # And show
        plt.show()


def task2():
    print('Task 2')

    # Transform a)
    def Ta(p):
        return 255 - p

    # Transform b)
    c = 1
    gamma = 0.8

    def Tb(p):
        return int(c * p ** gamma)

    # For each image file
    for img_txt in gray_txts:
        # Read image
        img = cv2.imread('images/' + img_txt, 0)

        # Calculate each gray scale
        out1 = gray_transform(img, Ta)
        out2 = gray_transform(img, Tb)

        hist_img = cv2.calcHist([img],  [0], None, [256], [0, 256])
        hist_out1 = cv2.calcHist([out1], [0], None, [256], [0, 256])
        hist_out2 = cv2.calcHist([out2], [0], None, [256], [0, 256])

        # Subplot images
        plt.subplot(231), plt.imshow(img, 'gray'), plt.title('Original')
        plt.subplot(232), plt.imshow(out1, 'gray'), plt.title('Gray 1')
        plt.subplot(233), plt.imshow(out2, 'gray'), plt.title('Gray 2')

        # Subplot histograms
        plt.subplot(234), plt.plot(hist_img)
        plt.subplot(235), plt.plot(hist_out1)
        plt.subplot(236), plt.plot(hist_out2)

        # Resize window
        mngr = plt.get_current_fig_manager()
        mngr.window.setGeometry(50, 100, 1200, 545)

        # And show
        plt.show()


def task3():
    print('Task 3')

    # A)

    # For each image file
    for img_txt in color_txts:
        # Read image
        img = cv2.imread('images/' + img_txt)

        # Kernels in use
        k1 = np.ones((3, 3), np.float32)/9
        k2 = np.array([[1, 2, 1], [2, 4, 2], [1, 2, 1]], np.float32)/16

        # Split and convolute each channel
        bgr = cv2.split(img)

        bc1, gc1, rc1 = tuple(map(lambda c: np.uint8(conv(c, k1)), bgr))
        bc2, gc2, rc2 = tuple(map(lambda c: np.uint8(conv(c, k2)), bgr))

        # Then merge
        out1 = cv2.merge((bc1, gc1, rc1))
        out2 = cv2.merge((bc2, gc2, rc2))

        # Subplot them
        plt.subplot(131), plt.imshow(img), plt.title('Original')
        plt.subplot(132), plt.imshow(out1), plt.title('conv k1')
        plt.subplot(133), plt.imshow(out2), plt.title('conv k2')

        # Resize window
        mngr = plt.get_current_fig_manager()
        mngr.window.setGeometry(50, 100, 1200, 545)

        # And show
        plt.show()

    # B)

    # For each image file
    for img_txt in gray_txts:
        # Read image
        img = cv2.imread('images/' + img_txt, 0)

        # Kernels in use
        sx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], np.int8)
        sy = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]], np.int8)

        # Convolute each gradient
        sobelx = conv(img, sx)
        sobely = conv(img, sy)

        # Calculate total gradient, and convert to right scaling
        grad = np.sqrt(np.power(sobelx, 2) + np.power(sobely, 2))
        out = np.uint8(grad * 255 / grad.max())

        # Subplot them
        plt.subplot(221), plt.imshow(img, 'gray'), plt.title('Original')
        plt.subplot(222), plt.imshow(out, 'gray'), plt.title('gradient')
        plt.subplot(223), plt.imshow(sobelx, 'gray'), plt.title('Sx')
        plt.subplot(224), plt.imshow(sobely, 'gray'), plt.title('Sy')

        # Resize window
        mngr = plt.get_current_fig_manager()
        mngr.window.setGeometry(50, 100, 1200, 545)

        # And show
        plt.show()


if __name__ == '__main__':
    task1()
    task2()
    task3()
