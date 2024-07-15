#pragma once
#include <map>
#include <memory>
#include <opencv2/opencv.hpp>

using namespace std;

class Image {
  friend std::ostream& operator<<(std::ostream& os, const Image& obj);

 private:
  cv::Mat image;
  std::string img_dir;

 public:
  explicit Image(std::string imagePath);
  Image(cv::Mat img);
  ~Image();
  void display();
  cv::Mat get_image();
  int get_rows();
  int get_cols();
};
