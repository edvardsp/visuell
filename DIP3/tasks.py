from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

# Import libraries
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
    # mng.window.showMaximized()
    mng.resize(1600, 1000)


def showImages(imgs, titles, seeds=None):
    diff = len(imgs) - len(titles)
    if diff > 0:
        titles += [''] * diff

    for i, img in enumerate(imgs):
        plt.subplot(1, len(imgs), i+1), plt.imshow(img, cmap='gray')
        plt.title(titles[i]), plt.xticks([]), plt.yticks([])

        if i == 0 and seeds is not None:
            fig = plt.gcf()
            for seed in seeds:
                c = plt.Circle(seed, 5., color='r')
                fig.gca().add_artist(c)

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

    imgs = [img, thresh]
    titles = ['Original', 'Segmented T = %i' % T]
    showImages(imgs, titles)

    print('Done')


def task1b(img, seeds):
    print('Task 1B')

    threshold = 25
    region = us.segmentRegionGrow(img, seeds, threshold, 'neumann')

    imgs = [img, region]
    titles = ['Original', 'Region T = %i' % threshold]
    showImages(imgs, titles, seeds)

    print('Done')


def task2a(img):
    print('Task 2A')

    result = us.morphNoiseRemoval(img)

    imgs = [img, result]
    titles = ['Original', 'Morph Noise Removal']
    showImages(imgs, titles)

    print('Done')


def task2b(img):
    print('Task 2B')

    result = us.morphDistTransform(img)

    imgs = [img, result]
    titles = ['Original', 'Morph Chess Distance']
    showImages(imgs, titles)

    print('Done')


def task2c(img):
    print('Task 2C')

    result = us.morphExtractBound(img)

    imgs = [img, result]
    titles = ['Original', 'Morph Extract Bound']
    showImages(imgs, titles)

    print('Done')
