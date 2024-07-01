#include "Local_contrast_enhancement.h"
 
LocalContrastEnhancement::LocalContrastEnhancement(Image img) {
    original_image = img.get_image();
    enhanced_image = Mat(img.get_rows(), img.get_cols(), CV_8U);
    HEIGHT = img.get_rows();
    WIDTH = img.get_cols();
}

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

/*shrink the size of image*/
Mat LocalContrastEnhancement::downsample(const Mat &original_image)
{
     
    Mat downsampled_image = Mat::zeros(HEIGHT / 2, WIDTH / 2, CV_8U);
 
    for (int i = 0; i < WIDTH; i += 2)
        for (int j = 0; j < HEIGHT; j += 2)
            if (i%2 == 0 && j%2 == 0)
                downsampled_image.at<uchar>(j/2, i/2) = original_image.at<uchar>(j, i);
 
    return downsampled_image;
}

/*Clipped histogram equalization
* input : input image
* output : CHE image
*/
void LocalContrastEnhancement::HistEqualization(Mat &img) {
    int clip{ 0 };
    int idx{ 0 };  
    Mat result;
    int* px = new int[256]{};
    int* px_copy = new int[256]{};
    float* CDF = new float[256]{};
    float* normalize = new float[256]{};

    //find PDF
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            idx = img.at<uchar>(i, j);
            px[idx] ++;
        }
    }
    cout << "find pdf" << endl;
    px_copy = px;

    int exceed_count{ 0 };

    //find clip value that makes the exceeded number of pixel same as not exceeded number of pixel. 
    while (exceed_count < (HEIGHT * WIDTH) / 2) {
        for (int i = 0; i < 256; i++) {
            if (px_copy[i] > 0) {
                px_copy[i] -= 1;
                exceed_count += 1;
            }
            if (exceed_count == (HEIGHT * WIDTH) / 2) {

                break;
            }
        }
        clip += 1;
    }
    cout << "find clip" << endl;
    //find total exceeded pixels 
    int exceed = 0;
    for (int i = 0; i < 256; i++) {
        if (px[i] > clip) {

            exceed += px[i] - clip;
            px[i] = clip;
        }
    }

    //distribute exceeded pixels over the bins
    while (exceed > 0) {
        for (int j = 0; j < 256; j++) {
            if (px[j] > 0) {

                px[j] += 1;
                exceed -= 1;

            }
            if (exceed == 0) { break; }
        }
    }
    cout << "distribute exceeded pixels" << endl;
    // calculate CDF corresponding to px 
    int accumulation = 0;
    for (int i = 0; i < 256; i++) {
        accumulation += px[i];
        CDF[i] = accumulation;
    }
    cout << "distribute exceeded pixels ended" << endl;
    // using general histogram equalization formula 
 
    for (int i = 0; i < 256; i++) {
        normalize[i] = ((CDF[i] - CDF[0]) / (img.rows * img.cols - CDF[0])) * 255;
        normalize[i] = static_cast<uchar>(normalize[i]);
    }
    cout << "normalize ended" << endl;
    /*Mat enhanced_image(img.rows, img.cols, CV_8U);*/
    cout << "normalize ended1" << endl;
    Mat_<uchar>::iterator it_out = enhanced_image.begin<uchar>();
    cout << "normalize ended2" << endl; 
    Mat_<uchar>::iterator it_ori = img.begin<uchar>();
    cout << "normalize ended3" << endl;
    Mat_<uchar>::iterator itend_ori = img.end<uchar>();

    cout << "enhanced_image assignment" << endl;

    for (; it_ori != itend_ori; it_ori++) {

        uchar pixel_value = static_cast<uchar>(*it_ori);
        *it_out = normalize[pixel_value];
        it_out++;
    }                

    delete[] px; 
    delete[] normalize;
    delete[] CDF;

};

/*add extra rows and cols (padding)*/
//input : full size image, size of filter
//output : padded image.
void LocalContrastEnhancement::symmetric_boundary(Mat &image, int scale) {

    int height_symm = image.rows;
    int width_symm = image.cols;

    int extra{ (scale - 1) / 2 };
    cv::Mat image_symmetric = cv::Mat::zeros(height_symm + 2 * extra, width_symm + 2 * extra, CV_8U);

    image.copyTo(image_symmetric(cv::Rect(extra, extra, width_symm, height_symm)));

    for (int i = 0; i < extra; i++) {
        //first row
        image.rowRange(i, i + 1).copyTo(image_symmetric(Range(i, i + 1), Range(extra, width_symm + extra)));
        //first column
        image.colRange(i, i + 1).copyTo(image_symmetric(Range(extra, height_symm + extra), Range(i, i + 1)));
        //last row
        image.rowRange(height_symm - (i + 1), height_symm - i).copyTo(image_symmetric(Range((height_symm + extra * 2) - (i + 1), (height_symm + extra * 2) - i), Range(extra, width_symm + extra)));
        //last column
        image.colRange(width_symm - (i + 1), width_symm - i).copyTo(image_symmetric(Range(extra, height_symm + extra), Range((width_symm + extra * 2) - (i + 1), (width_symm + extra * 2) - i)));
        //top left corner
        image(Range(0, extra), Range(0, extra)).copyTo(image_symmetric(Range(0, extra), Range(0, extra)));
        //bottom left corner
        image(Range(height_symm - extra, height_symm), Range(0, extra)).copyTo(image_symmetric(Range(height_symm + extra, height_symm + extra * 2), Range(0, extra)));
        //top right corner
        image(Range(0, extra), Range(width_symm - extra, width_symm)).copyTo(image_symmetric(Range(0, extra), Range(width_symm + extra, width_symm + extra * 2)));
        //bottom right corner
        image(Range(height_symm - extra, height_symm), Range(width_symm - extra, width_symm)).copyTo(image_symmetric(Range(height_symm + extra, height_symm + extra * 2), Range(width_symm + extra, width_symm + extra * 2)));
    }
    image = image_symmetric;
}

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

    auto duration_roiImage_sum = 0;
    for (int i = scale  / 2; i < image.cols - scale / 2; i++) {
        for (int j = scale / 2; j < image.rows - scale / 2; j++) {
             
            Mat roiImage(image, Rect(i - scale / 2, j - scale / 2, scale, scale));
              
            output.at<uchar>(j - scale / 2, i - scale / 2) = convolution(roiImage);
        }
    }
    return output;
}

/*expand the size of image*/
Mat LocalContrastEnhancement::upsample(const Mat &original_image)
{
    Mat upsampled_image = Mat::zeros(HEIGHT, WIDTH, CV_8U);
    for (int i = 0; i < WIDTH; i += 2)
    {
        for (int j = 0; j < HEIGHT; j += 2)
        {
            upsampled_image.at<uchar>(j, i) = original_image.at<uchar>(j / 2, i / 2);
        }
    }
    return upsampled_image;
}

/*find ROLP (ratio of lowpass)*/
Mat LocalContrastEnhancement::rolp(const Mat &original_image, const Mat &expanded)
{
    Mat output(original_image.rows, original_image.cols, CV_32FC1); 
    Mat float_expanded;
    Mat float_original_image;

    expanded.convertTo(float_expanded, CV_32FC1);
    original_image.convertTo(float_original_image, CV_32FC1);

    // First, I must loop through and multiply each element in the original_image by 4.0
    // but stackoverflow says that opencv supports scalar and matrix multiplication
     //expanded_times4 = 4.0 * expanded;
    // Now, divide each element of the original image by its respective element in output and assign the result to output
    //cv::divide(original_image, expanded_times4, output); // for some reason this just results in a black image
    // so maybe I'll write the element-wise division from scratch instead
    // iterate through each pixel in original_image, divide it by its respective pixel in expanded_times4, and store the result in the respective pixel of output
    for (int j = 0; j < original_image.rows; j++) {
        for (int i = 0; i < original_image.cols; i++) {

            output.at<float>(j, i) = float_original_image.at<float>(j, i) / (4.0 * float_expanded.at<float>(j, i));

            // this had division by 0, but it's probably because we haven't added the boundary_sym function yet, so there are zeroes at the boundaries
            // update: we've added the symmetric boundary function but it's still claiming error: unhandled excpetion 0xC0000094: Integer deivision, which is division by zero
            // so there must be some pixel in expanded_times4 that is zero
        }
    }

    return output;
}

void LocalContrastEnhancement::adjustment(Mat &original_image, const Mat &rolp_result)
{
    int final_value{ 0 };
    Mat CEG256 (original_image.rows, original_image.cols, CV_8U);
    uchar CE_EXP;
    int count = 0;
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
