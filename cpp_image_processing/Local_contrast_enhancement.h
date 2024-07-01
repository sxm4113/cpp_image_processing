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

	void HistEqualization(Mat &img);
	void adjustment(Mat &original_image, const Mat &rolp_result);
	Mat rolp(const Mat &original_image, const Mat &expanded);
	Mat upsample(const Mat &original_image);
	Mat downsample(const Mat &original_image); 
	Mat convolution_image(const Mat &image );
	void symmetric_boundary(Mat &image, int scale);
	void run_algorithm();

	Mat get_enhanced_image();  
};
#endif  //_LOCAL_CONTRAST_ENHANCEMENT_H_