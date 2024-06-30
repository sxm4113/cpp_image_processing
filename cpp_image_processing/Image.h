#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <opencv2/opencv.hpp>
#include <map>
#include <memory>

using namespace cv;
using namespace std;

class Image {
    friend ostream& operator<<(ostream& os, const Image& obj);

private: Mat image;
       string img_dir; 
public: Image();
        Image(string imagePath);
        ~Image();
        void display();
        Mat get_image();
        int get_rows();
        int get_cols();

};
#endif //_IMAGE_H_