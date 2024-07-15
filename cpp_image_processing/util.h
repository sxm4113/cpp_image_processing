#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

/*add extra rows and cols (padding)*/
//input : full size image, size of filter
//output : padded image.
 
void symmetric_boundary(Mat &image, int scale) {
 
    int height_symm = image.rows;
    int width_symm = image.cols;

    int extra{ (scale - 1) / 2 };
    Mat image_symmetric = Mat::zeros(height_symm + 2 * extra, width_symm + 2 * extra, CV_8U);

    image.copyTo(image_symmetric(Rect(extra, extra, width_symm, height_symm)));

    for (int i = 0; i < extra; i++) {
        //first row
        image.rowRange(i, i + 1).copyTo(image_symmetric(Range(i, i + 1), Range(extra, width_symm + extra)));
        //first column
        image.colRange(i, i + 1).copyTo(image_symmetric(Range(extra, height_symm + extra), Range(i, i + 1)));
        //last row
        image.rowRange(height_symm - (i + 1), height_symm - i).copyTo(image_symmetric(Range((height_symm + extra * 2) - (i + 1), (height_symm + extra * 2) - i), Range(extra, width_symm + extra)));
        //last column
        image.colRange(width_symm - (i + 1), width_symm - i).copyTo(image_symmetric(Range(extra, height_symm + extra), Range((width_symm + extra * 2) - (i + 1), (width_symm + extra * 2) - i)));
        //top left corner
        image(Range(0, extra), Range(0, extra)).copyTo(image_symmetric(Range(0, extra), Range(0, extra)));
        //bottom left corner
        image(Range(height_symm - extra, height_symm), Range(0, extra)).copyTo(image_symmetric(Range(height_symm + extra, height_symm + extra * 2), Range(0, extra)));
        //top right corner
        image(Range(0, extra), Range(width_symm - extra, width_symm)).copyTo(image_symmetric(Range(0, extra), Range(width_symm + extra, width_symm + extra * 2)));
        //bottom right corner
        image(Range(height_symm - extra, height_symm), Range(width_symm - extra, width_symm)).copyTo(image_symmetric(Range(height_symm + extra, height_symm + extra * 2), Range(width_symm + extra, width_symm + extra * 2)));
    }
 
    image = image_symmetric; 
}