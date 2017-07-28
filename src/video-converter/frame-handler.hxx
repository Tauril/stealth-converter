#include <opencv2/imgproc/imgproc.hpp>
#include "helper.hh"

namespace convert
{
    inline void FrameHandler::squarify(cv::Mat& img)
    {
        if (img.rows == img.cols)
            return;
        ssize_t top = 0;
        ssize_t bottom = 0;
        ssize_t left = 0;
        ssize_t right = 0;
        ssize_t side = 0;

        if (img.rows > img.cols) {
            left = (img.rows - img.cols) / 2;
            right = (img.rows - img.cols) / 2;
            side = img.rows;
        }
        else {
            top = (img.cols - img.rows) / 2;
            bottom = (img.cols - img.rows) / 2;
            side = img.cols;
        }
        cv::Mat square_img(side, side, CV_8UC3);
        copyMakeBorder(img, square_img, top, bottom, left, right,
                       cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
        img = square_img;
    }
}