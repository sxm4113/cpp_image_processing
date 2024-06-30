#include "Image.h"

ostream& operator<< (ostream & os, const Image& obj) {
    os << " Image: " << obj.img_dir;
    return os;
}

//no-args constructor
Image::Image()
    :img_dir{}, image {}  {}

Image::Image(string imagePath) {
    img_dir = imagePath;
    image = cv::imread(imagePath,IMREAD_GRAYSCALE);  
}
     
//descructor
Image::~Image() {}

//display image
void Image::display() {
    cv::imshow("Display Window", image);
}

int Image::get_rows() { return image.rows; }
int Image::get_cols() { return image.cols; }
Mat Image::get_image() { return image; }