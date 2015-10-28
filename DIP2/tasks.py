from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

from matplotlib import pyplot as plt

import utils as us

##########################################################################
#
#   Tasks
#
##########################################################################


def task1a(img):
    img_gauss = us.fftConvoluteKernel(img, us.Gaussian)
    img_lapl = us.fftConvoluteKernel(img, us.Laplacian)

    plt.subplot(131), plt.imshow(img, cmap='gray')
    plt.title('Input Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(132), plt.imshow(img_gauss, cmap='gray')
    plt.title('Gaussian LPF'), plt.xticks([]), plt.yticks([])
    plt.subplot(133), plt.imshow(img_lapl, cmap='gray')
    plt.title('Laplacian HPF'), plt.xticks([]), plt.yticks([])

    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()

    print('Task 1A Done')
    plt.show()


def task1b(img):
    img_sharp = us.fftSharpImage(img)

    plt.subplot(121), plt.imshow(img, cmap='gray')
    plt.title('Input Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(122), plt.imshow(img_sharp, cmap='gray')
    plt.title('Sharpened Image'), plt.xticks([]), plt.yticks([])

    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()

    print('Task 1B Done')
    plt.show()


def task1c(img1, img2):
    img_hybrid = us.fftMakeHybrid(img1, img2)

    plt.subplot(131), plt.imshow(img1, cmap='gray')
    plt.title('Input Image 1'), plt.xticks([]), plt.yticks([])
    plt.subplot(132), plt.imshow(img2, cmap='gray')
    plt.title('Input Image 2'), plt.xticks([]), plt.yticks([])
    plt.subplot(133), plt.imshow(img_hybrid, cmap='gray')
    plt.title('Hybrid Image'), plt.xticks([]), plt.yticks([])

    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()

    print('Task 1C Done')
    plt.show()


def task2a(img):
    img_alias = us.downSample(img)
    img_aliasN = img
    for _ in range(3):
        img_aliasN = us.downSample(img_aliasN)

    plt.subplot(131), plt.imshow(img, cmap='gray')
    plt.title('Input Image 1'), plt.xticks([]), plt.yticks([])
    plt.subplot(132), plt.imshow(img_alias, cmap='gray')
    plt.title('Downsampled'), plt.xticks([]), plt.yticks([])
    plt.subplot(133), plt.imshow(img_aliasN, cmap='gray')
    plt.title('Downsampled N times'), plt.xticks([]), plt.yticks([])

    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()

    print('Task 2A Done')
    plt.show()


def task2b(img):
    img_gauss = us.fftConvoluteKernel(img, us.Gaussian)

    img_alias = us.downSample(img)
    img_galias = us.downSample(img_gauss)

    img_aliasN = img
    img_galiasN = img_gauss
    for _ in range(4):
        img_aliasN = us.downSample(img_alias)
        img_galiasN = us.downSample(img_galiasN)

    plt.subplot(131), plt.imshow(img, cmap='gray')
    plt.title('Input Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(132), plt.imshow(img_alias, cmap='gray')
    plt.title('Downsample'), plt.xticks([]), plt.yticks([])
    plt.subplot(133), plt.imshow(img_galias, cmap='gray')
    plt.title('Gauss + Downsample'), plt.xticks([]), plt.yticks([])

    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()

    plt.show()

    plt.subplot(131), plt.imshow(img, cmap='gray')
    plt.title('Input Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(132), plt.imshow(img_aliasN, cmap='gray')
    plt.title('Downsample N times'), plt.xticks([]), plt.yticks([])
    plt.subplot(133), plt.imshow(img_galiasN, cmap='gray')
    plt.title('Gauss + Downsample N times'), plt.xticks([]), plt.yticks([])

    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()

    plt.show()
    print('Task 2B Done')
