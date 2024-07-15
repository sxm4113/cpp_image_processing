#include "Local_contrast_enhancement.h"
#include "util.h"

using namespace cv;

LocalContrastEnhancement::LocalContrastEnhancement(Image &img)
    : original_image{img.get_image()},
      enhanced_image{Mat(img.get_rows(), img.get_cols(), CV_8U)},
      HEIGHT{img.get_rows()},
      WIDTH{img.get_cols()} {}

void LocalContrastEnhancement::run_algorithm() {
  Mat downsampled_image = downsample(original_image);
  symmetric_boundary(downsampled_image, 5);
  Mat down_conv_filtered = convolution_image(downsampled_image);
  Mat up_sampled = upsample(down_conv_filtered);
  symmetric_boundary(up_sampled, 5);
  Mat up_conv_filtered = convolution_image(up_sampled);
  cout << "convolution finished" << endl;
  Mat output = rolp(original_image, up_conv_filtered);
  cout << "rolp finished" << endl;
  adjustment(original_image, output);
};

Mat LocalContrastEnhancement::get_enhanced_image() {return enhanced_image;}
 
/*calculate convolution*/
//input : 5 by 5 image, filter
//output : pixel value of the center of 5 by 5 image. 
Mat LocalContrastEnhancement::convolution_image(const Mat &image )
{
    int scale = 5;
    auto convolution=[](Mat sub_image)
    {
        std::vector<std::vector <double>> kernel = { {1.0 / 256, 4.0 / 256, 6.0 / 256, 4.0 / 256, 1.0 / 256} ,
                                {4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256},
                                {6.0 / 256, 24.0 / 256, 36.0 / 256, 24.0 / 256, 6.0 / 256},
                                {4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256},
                                {1.0 / 256, 4.0 / 256, 6.0 / 256, 4.0 / 256, 1.0 / 256 } };

        double convolution_value{ 0 };
        for (int j = 0; j < sub_image.rows; j++) {
            for (int i = 0; i < sub_image.cols; i++) { 

                convolution_value = convolution_value + sub_image.at<uchar>(j, i) * kernel.at(j).at(i);
                
            }
        }
         
        if (convolution_value < 1)
            return 1;
        else return int(convolution_value);
    }; 

    Mat output(HEIGHT, WIDTH, CV_8U); 
 
    for (int i = scale  / 2; i < image.cols - scale / 2; i++) {
        for (int j = scale / 2; j < image.rows - scale / 2; j++) {
             
            Mat roiImage(image, Rect(i - scale / 2, j - scale / 2, scale, scale));
              
            output.at<uchar>(j - scale / 2, i - scale / 2) = convolution(roiImage);
        }
    }
    return output;
}

/*find ROLP (ratio of lowpass)*/
Mat LocalContrastEnhancement::rolp(const Mat &original_image, const Mat &expanded)
{
    Mat output(original_image.rows, original_image.cols, CV_32FC1); 
    Mat float_expanded;
    Mat float_original_image;

    expanded.convertTo(float_expanded, CV_32FC1);
    original_image.convertTo(float_original_image, CV_32FC1);

    for (int j = 0; j < original_image.rows; j++) {
        for (int i = 0; i < original_image.cols; i++) {

            output.at<float>(j, i) = float_original_image.at<float>(j, i) / (4.0 * float_expanded.at<float>(j, i));

        }
    }

    return output;
}

void LocalContrastEnhancement::adjustment(Mat &original_image, const Mat &rolp_result)
{
    int final_value{ 0 };
    Mat CEG256 (original_image.rows, original_image.cols, CV_8U);
    uchar CE_EXP; 
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            if (i > 1 && i < WIDTH - 1 && j >1 && j < HEIGHT) {

                if (rolp_result.at<float>(j, i) < 1)
                {
            
                    uchar minimum = original_image.at<uchar>(j - 1, i - 1);
                    for (int original_j = j - 1; original_j < j + 1; original_j++)
                    {
                        for (int original_i = i - 1; original_i < i + 1; original_i++)
                        {
                            if (original_image.at<uchar>(original_j, original_i) < minimum)
                            {
                                minimum = original_image.at<uchar>(original_j, original_i);
                            }
                        }
                    } 
                    CE_EXP = minimum;
                }
                else if (rolp_result.at<float>(j, i) > 1)
                {
                    uchar maximum = original_image.at<uchar>(j - 1, i - 1);

                    for (int original_j = j - 1; original_j < j + 1; original_j++)
                    {
                        for (int original_i = i - 1; original_i < i + 1; original_i++)
                        {
                            if (original_image.at<uchar>(original_j, original_i) > maximum)
                            {
                                maximum = original_image.at<uchar>(original_j, original_i);
                            }
                        }
                    } 
                    CE_EXP = maximum;
                }
                else CE_EXP = original_image.at<uchar>(j, i);
            }
            else CE_EXP = original_image.at<uchar>(j, i);
             
            final_value = trunc(rolp_result.at<float>(j, i) * 0.8 * CE_EXP);
              
            if (final_value > 255)
                final_value = 255;

            CEG256.at<uchar>(j,i) = final_value;

        }
    }
    enhanced_image=CEG256;
}
