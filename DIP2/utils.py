from __future__ import print_function

##########################################################################
#
#   Imports
#
##########################################################################

import cv2
import numpy as np

##########################################################################
#
#   Globals
#
##########################################################################

temp = cv2.getGaussianKernel(5, 10)
Gaussian = temp * temp.T
del temp

Laplacian = np.array([[0, -1, 0],
                      [-1, 4, -1],
                      [0, -1, 0]])

##########################################################################
#
#   Functions
#
##########################################################################


def makeSize(img, shape):
    rows, cols = shape
    crows, ccols = img.shape

    right = cols - ccols
    bottom = rows - crows
    bordertype = cv2.BORDER_CONSTANT

    return cv2.copyMakeBorder(img, 0, bottom, 0, right, bordertype, value=0)


def makeOptimalSize(img):
    opt_shape = map(cv2.getOptimalDFTSize, img.shape)
    return makeSize(img, opt_shape)


def getSpectrum(img):
    flags = cv2.DFT_COMPLEX_OUTPUT | cv2.DFT_SCALE
    img_dft = cv2.dft(np.float32(img), flags=flags)
    return np.fft.fftshift(img_dft)


def invertSpectrum(spectrum):
    spectrum_ishift = np.fft.ifftshift(spectrum)
    img_back = cv2.idft(spectrum_ishift)
    return cv2.magnitude(img_back[:, :, 0], img_back[:, :, 1])


def convolute(spec1, spec2):
    flags = cv2.DFT_COMPLEX_OUTPUT | cv2.DFT_SCALE
    return cv2.mulSpectrums(spec1, spec2, flags=flags)


def fftConvoluteKernel(img, kernel):
    # Pad the kernel
    mask_pad = makeSize(kernel, img.shape)

    # shifted FFT of image and kernel
    img_shift = getSpectrum(img)
    mask_shift = getSpectrum(mask_pad)

    # Convolution of image and kernel by multiplication
    f_shift = convolute(img_shift, mask_shift)

    # Shift back and get the inverse FFT
    f = invertSpectrum(f_shift)
    return np.uint8(f * 255 / f.max())


def fftSharpImage(img):
    rows, cols = img.shape
    img_hpf = fftConvoluteKernel(img, Laplacian)
    sharp = cv2.add(img, img_hpf)

    return sharp


def fftMakeHybrid(img1, img2):
    img_hpf = fftConvoluteKernel(img1, Laplacian)
    img_lpf = fftConvoluteKernel(img2, Gaussian)
    return cv2.add(img_hpf, img_lpf)


def downSample(img):
    return img[0::2, 0::2]
