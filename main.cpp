// MMX example
#include <x86intrin.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

// Computes the x component of the gradient vector
// at a given point in a image.
// returns gradient in the x direction
int xGradientAsm(Mat image, int x, int y) {
    _mm_empty();
    __m64 part1 = _mm_cvtsi32_si64((int) image.at<uchar>(y - 1, x - 1));
    __m64 part2 = _mm_cvtsi32_si64((int) image.at<uchar>(y, x - 1));
    __m64 part3 = _mm_cvtsi32_si64((int) image.at<uchar>(y + 1, x - 1));
    __m64 part4 = _mm_cvtsi32_si64((int) image.at<uchar>(y - 1, x + 1));
    __m64 part5 = _mm_cvtsi32_si64((int) image.at<uchar>(y, x + 1));
    __m64 part6 = _mm_cvtsi32_si64((int) image.at<uchar>(y + 1, x + 1));
    __m64 multiplier = _mm_cvtsi32_si64(2);
    __m64 part2s = _mm_mullo_pi16(part2, multiplier);
    __m64 part5s = _mm_mullo_pi16(part2, multiplier);
    __m64 gx = _mm_add_pi32(part1, part2s);
    __m64 gx1 = _mm_add_pi32(gx, part3);
    __m64 gx2 = _mm_sub_pi32(gx1, part4);
    __m64 gx3 = _mm_sub_pi32(gx2, part5s);
    __m64 gx4 = _mm_sub_pi32(gx3, part6);


    return _mm_cvtsi64_si32(gx4);
}

 
// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction
int yGradientAsm(Mat image, int x, int y) {
    _mm_empty();
    __m64 part1 = _mm_cvtsi32_si64((int) image.at<uchar>(y - 1, x - 1));
    __m64 part2 = _mm_cvtsi32_si64((int) image.at<uchar>(y - 1, x));
    __m64 part3 = _mm_cvtsi32_si64((int) image.at<uchar>(y - 1, x + 1));
    __m64 part4 = _mm_cvtsi32_si64((int) image.at<uchar>(y + 1, x - 1));
    __m64 part5 = _mm_cvtsi32_si64((int) image.at<uchar>(y + 1, x));
    __m64 part6 = _mm_cvtsi32_si64((int) image.at<uchar>(y + 1, x + 1));
    __m64 multiplier = _mm_cvtsi32_si64(2);
    __m64 part2s = _mm_mullo_pi16(part2, multiplier);
    __m64 part5s = _mm_mullo_pi16(part2, multiplier);
    __m64 gx = _mm_add_pi32(part1, part2s);
    __m64 gx1 = _mm_add_pi32(gx, part3);
    __m64 gx2 = _mm_sub_pi32(gx1, part4);
    __m64 gx3 = _mm_sub_pi32(gx2, part5s);
    __m64 gx4 = _mm_sub_pi32(gx3, part6);

    return  _mm_cvtsi64_si32(gx4);
}


int main(int argc, char* argv[]) {
    char const *src_file = "lena.bmp";
	char const *proc_file = "lena_proc.bmp";
	char const *proc2_file = "lena_proc2.bmp";

    Mat src, dst, asmDst;
    int gx, gy, sum;
 
      // Load an image
    src = imread(src_file, CV_LOAD_IMAGE_GRAYSCALE);
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
     
    if( !src.data )
    { return -1;  }
 
 
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    // Gradient X
    Sobel(src, grad_x, CV_16S, 1, 0);
    // Gradient Y
    Sobel(src, grad_y, CV_16S, 0, 1);

    convertScaleAbs( grad_x, abs_grad_x );
    convertScaleAbs( grad_y, abs_grad_y );
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);

    asmDst = src.clone();
    for (int y = 1; y < src.rows - 1; y++) {
        for (int x = 1; x < src.cols - 1; x++) {
            gx = xGradientAsm(src, x, y);
            gy = yGradientAsm(src, x, y);
            sum = abs(gx) + abs(gy);
            sum = sum > 255 ? 255 : sum;
            sum = sum < 0 ? 0 : sum;
            asmDst.at<uchar>(y,x) = sum;
        }
    }

 
    namedWindow("C++ Sobel");
    imshow("C++ Sobel", dst);
    imwrite(proc_file, dst);

    namedWindow("Assembly Sobel");
    imshow("Assembly Sobel", asmDst);
    imwrite(proc2_file, asmDst);

 
    namedWindow("initial");
    imshow("initial", src);
 
    waitKey(0);

    return 0;
}

