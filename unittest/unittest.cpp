#include "../cpp_image_processing/Image.cpp"
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

