#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.
    if(kernelSize<=0) {
        return;
    }
    GrayscaleImage tempImage(image);
    int startIndex = kernelSize/2;
    for(int i= 0;i<image.get_width();i++) {
        for(int j= 0;j<image.get_height();j++) {
            int sum = 0;
            for(int g = i-startIndex;g<i+startIndex+1;g++) {
                for(int k=j-startIndex;k<j+startIndex+1;k++) {
                    if(g<image.get_height()&&g>=0&&k>=0&&k<image.get_width()){
                        sum+=tempImage.get_pixel(g,k);
                    }
                }
            }
            image.get_data()[i][j] = sum/(kernelSize*kernelSize);
        }
    }

}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.
    if(kernelSize<=0) {
        return;
    }
    double **gausKernel = new double *[kernelSize];
    for(int i=0;i<kernelSize;i++) {
        gausKernel[i] = new double[kernelSize];
    }

    const double coef = 1.0 / (2.0 * M_PI * sigma * sigma);
    const int middle = kernelSize/2;
    double sum = 0.0;
    for (int i = 0; i<kernelSize;i++) {
        for (int j = 0; j<kernelSize;j++) {
            const double exponent = -(((j-middle)*(j-middle)+(i-middle)*(i-middle))/(2*sigma*sigma));
            gausKernel[i][j] = coef*exp(exponent);
            sum += gausKernel[i][j];
        }
    }

    for (int i = 0;i<kernelSize;i++) {
        for (int j = 0;j<kernelSize;j++) {
            gausKernel[i][j] /= sum;
        }
    }

    GrayscaleImage tempImage(image);
    const int startIndex = kernelSize/2;
    for(int i= 0;i<image.get_width();i++) {
        for(int j= 0;j<image.get_height();j++) {
            double sum1 = 0;
            int row=0;
            for(int g = i-startIndex;g<i+startIndex+1;g++) {
                int col = 0;
                for(int k=j-startIndex;k<j+startIndex+1;k++) {
                    if(g<image.get_height()&&g>=0&&k>=0&&k<image.get_width()){
                        sum1+= (tempImage.get_pixel(g, k) * gausKernel[row][col]);
                    }
                    col++;
                }
                row++;
            }
            if (sum1>255) {
                sum1 = 255;
            }else if (sum1<0) {
                sum1 = 0;
            }

            image.get_data()[i][j] = sum1;
        }
    }
    for (int i = 0;i<kernelSize;i++) {
        delete[] gausKernel[i];
    }
    delete[] gausKernel;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].
    if(kernelSize<=0) {
        return;
    }
    GrayscaleImage blurImage(image);
    apply_gaussian_smoothing(blurImage,kernelSize);

    for(int i = 0;i<image.get_height();i++) {
        for(int j = 0;j<image.get_width();j++) {
            double unsharp = (image.get_pixel(i,j)-blurImage.get_pixel(i,j))*amount+image.get_pixel(i,j);
            if(unsharp>=255) {
                unsharp=255;
            }else if(unsharp<=0) {
                unsharp=0;
            }
            image.get_data()[i][j] = unsharp;
        }
    }
}
