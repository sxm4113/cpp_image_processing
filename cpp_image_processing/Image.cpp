#include "Image.h"

std::ostream& operator<< (std::ostream & os, const Image& obj) {
    os << " Image: " << obj.img_dir;
    return os;
}
 
Image::Image(std::string imagePath) {
    img_dir = imagePath;
    image = cv::imread(imagePath,cv::IMREAD_GRAYSCALE);  
}
     
//descructor
Image::~Image() {}

//display image
void Image::display() {
    cv::imshow("Display Window", image);
}

int Image::get_rows() { return image.rows; }
int Image::get_cols() { return image.cols; }
cv::Mat Image::get_image() { return image; }