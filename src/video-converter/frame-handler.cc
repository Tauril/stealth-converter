#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "frame-handler.hh"
#include "helper.hh"

namespace convert
{

    cv::Mat* FrameHandler::process(std::vector<std::string> inputs)
    {
        std::vector<cv::Mat> frames;
        // Read the 6 input images
        for (auto img : inputs) {
            cv::Mat image = cv::imread(img, CV_LOAD_IMAGE_COLOR);
            if (!image.data) {
                std::cerr << "Error: cannot open image, wrong file name" << std::endl;
                return nullptr;
            }
            frames.push_back(image);
        }
        return process(frames);
    }

    cv::Mat* FrameHandler::process(cv::Mat input)
    {
        std::vector<cv::Mat> vect;
        vect.push_back(input);
        return process(vect);
    }

    cv::Mat* FrameHandler::process(std::vector<cv::Mat> inputs)
    {
        assert(inputs.size() == 1 || inputs.size() == 6);

        std::vector<cv::Mat> frames;
        size_t width = 0;
        // Read the 6 input images
        for (auto image : inputs) {
            squarify(image);
            width = image.rows;
            frames.push_back(image);
        }

        Converter cvrt(width, constants::m_pi, 2.0 * constants::m_pi);

        cv::Mat* output = new cv::Mat(cvrt.getPanoSizeV() , cvrt.getPanoSizeH(), CV_8UC3);

        // Map the pixels from the panorama back to the source image
        for (size_t i = 0; i < cvrt.getPanoSizeH(); ++i) {
            for (size_t j = 0; j < cvrt.getPanoSizeV(); ++j) {
                // Get the corresponding position of (i, j)
                auto coord = cvrt.getCoordinate(i, j);
                // output pixel
                cv::Vec3b& rgba = output->at<cv::Vec3b>(j, i);

                if (frames.size() == 1) {
                    if (coord->getCubeFace() == CubeFace::front) {
                        rgba = frames[0].at<cv::Vec3b>(coord->getY(), coord->getX());
                    }
                    else {
                        // For now no noise. Later if the video is properly converted
                        // noise will be inserted here.
                        rgba[0] = 0;
                        rgba[1] = 0;
                        rgba[2] = 0;
                    }
                }
                else {
                    int index = help::as_integer<CubeFace>(coord->getCubeFace());
                    const auto& toto = frames[index];
                    int y_coord = coord->getY();
                    int x_coord = coord->getX();
                    // if (x_coord > 640 || y_coord > 640)
                    //     std::cout << "y: " << y_coord << " x: " << x_coord << std::endl; 
                    rgba = toto.at<cv::Vec3b>(y_coord, x_coord);
                }
            }
        }
        return output;
    }


    void FrameHandler::process_video_file(const std::string& input_video)
    {
        cv::VideoCapture capture(input_video);
        cv::Mat frame;
        std::vector<cv::Mat*> outputs;
        cv::VideoWriter output_video;
        if (!capture.isOpened()) {
            std::cerr << "Error: cannot open input video. Please input a valid video file name" << std::endl;
            return;
        }
        bool opened = capture.read(frame);
        if (!opened) {
            std::cerr << "Error: empty video file" << std::endl;
            return;
        }
        cv::Size video_size = cv::Size(frame.cols, frame.rows);
        output_video.open("test_name.mp4",  0x00000021, capture.get(CV_CAP_PROP_FPS), video_size, true);
        while (opened) {
            output_video << *process(frame);
            opened = capture.read(frame);
        }
        capture.release();
        output_video.release();
    }

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
