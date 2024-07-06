#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream> 

#include "Image.h"
#include "Local_contrast_enhancement.h"

int main() {
  Image img{"../image/input.jpg"};
  std::cout << img << std::endl;
  LocalContrastEnhancement result{img};

  std::cout << "Run Algorithm .. ." << std::endl;
  result.run_algorithm();
  cv::imshow("Display Window", result.get_enhanced_image());
  cv::waitKey(0);

  return 0;
}
