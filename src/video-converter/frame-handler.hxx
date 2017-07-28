#include <opencv2/imgproc/imgproc.hpp>

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

    inline cv::Size FrameHandler::get_output_size(const cv::Mat& image)
    {
        auto cols = image.cols;
        auto rows = image.rows;
        auto width = (cols > rows) ? cols : rows;
        auto final_rows = width * 2;
        auto final_cols = width * 4;
        return cv::Size(final_cols, final_rows);
    }
}