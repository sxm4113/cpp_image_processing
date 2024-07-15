#include "../cpp_image_processing/Image.cpp"
#include "Local_contrast_enhancement.h"

#include "gtest/gtest.h"

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

TEST(LocalContrastEnhancementTest,symmetric_boundary)
{
	Mat Z = Mat::zeros(10,10);
	Image img(Z);
	LocalContrastEnhancement test_obj(img);
	test_obj.symmetric_boundary
	const int C.row = img.get_cols();
	EXPECT_EQ(peri,1280);

}

