#include "../cpp_image_processing/Image.cpp" 
#include "../cpp_image_processing/util.h" 

#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>

TEST(ImageTest,get_rows)
{
	Image img{"../image/input.jpg"};
	const int peri = img.get_rows();
	EXPECT_EQ(peri,1024);
}

TEST(ImageTest,get_cols)
{
	Image img{"../image/input.jpg"};
	const int peri = img.get_cols();
	EXPECT_EQ(peri,1280);

}
 
TEST(UtilTEST, symmetric_boundary)
{
	Mat z = Mat::zeros(10,10, CV_8UC1);
	Image test_obj(z);
	symmetric_boundary(z,5);
	const int col = z.cols;
	const int row = z.rows;

	EXPECT_EQ(col,14);
	EXPECT_EQ(row,14);
} 

TEST(UtilTEST, upsample)
{
	Mat z = Mat::zeros(9,9, CV_8UC1);
	Image test_obj(z);
	cv::Mat result = upsample(z);
	const int upsampled_col = result.cols;
	const int upsampled_row = result.rows;
	
	EXPECT_EQ(upsampled_col,9);
	EXPECT_EQ(upsampled_row,9);
} 
