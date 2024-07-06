#pragma once
#include <opencv2/opencv.hpp>

#include "Image.h"

class LocalContrastEnhancement {
private:
 cv::Mat original_image;
 cv::Mat enhanced_image;
 int HEIGHT;
 int WIDTH;

public:
	/// constructor</summary>
	explicit LocalContrastEnhancement(Image& img);

	void HistEqualization(cv::Mat &img);
	void adjustment(cv::Mat& original_image, const cv::Mat& rolp_result);
        cv::Mat rolp(const cv::Mat& original_image, const cv::Mat& expanded);
        cv::Mat upsample(const cv::Mat& original_image);
	cv::Mat downsample(const cv::Mat& original_image); 
	cv::Mat convolution_image(const cv::Mat& image );
	void symmetric_boundary(cv::Mat &image, int scale);
	void run_algorithm();

	cv::Mat get_enhanced_image();
};