#ifndef _LOCAL_CONTRAST_ENHANCEMENT_H_
#define _LOCAL_CONTRAST_ENHANCEMENT_H_
#include <opencv2/opencv.hpp>  
#include "Image.h"

using namespace cv;

class LocalContrastEnhancement {
private:
	Mat enhanced_image; 
	Mat original_image;
	int WIDTH;
	int HEIGHT;
public:
	/// constructor</summary>
	LocalContrastEnhancement(Image img);

	void HistEqualization(Mat img);
	Mat adjustment(Mat original_image, Mat rolp_result);
	Mat rolp(Mat original_image, Mat expanded);
	Mat upsample(Mat original_image);
	Mat downsample(Mat original_image); 
	Mat convolution_image(Mat image );
	Mat symmetric_boundary(Mat image, int scale);
	void run_algorithm();

	Mat get_enhanced_image();  
};
#endif  //_LOCAL_CONTRAST_ENHANCEMENT_H_