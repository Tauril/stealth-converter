#include <vector>
#include <string>
#include <cassert>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "frame-handler.hh"

namespace convert
{
    void FrameHandler::process(std::vector<std::string> inputs)
    {
        assert(inputs.size() == 1 || inputs.size() == 6);

        std::vector<const cv::Mat&> frames();
        size_t width = 0;
        // Read the 6 input images
        for (auto img : inputs) {
            cv::Mat image = cv::imread(img, cv::CV_LOAD_IMAGE_COLOR);
            if (!image.data) {
                printf( " No image data \n " );
                return;
            }
            // for now assume the pictures are squares
            assert(image.elemSize() == image.elemSize1());
            width = image.elemSize();
            frames.push_back(image);
        }

        /*
        Initialise the algorithm:
            the width of each input is 640 pixel,
            the vertical view portion is PI (180 degrees),
            the horizontal view portion is 2*PI (360 degrees).
        In this case, the output image size will be calculated accordingly.
        */
        Converter cvrt(width, constants::m_pi, 2.0 * constants::m_pi);

        cv::Mat output(cvrt.getPanoSizeV() , cvrt.getPanoSizeH(), cv::CV_8UC4);

        // Map the pixels from the panorama back to the source image
        for (size_t i = 0; i < cvrt.getPanoSizeH(); ++i) {
            for (size_t j = 0; j < cvrt.getPanoSizeV(); ++j) {
                // Get the corresponding position of (i, j)
                auto coord = cvrt.getCoordinate(i, j);
                // output pixel
                cv::Vec4b& rgba = output.at<cv::Vec4b>(i, j);

                if (frames.size() == 1) {
                    if (coord.getCubeFace() == CubeFace::front) {
                        rgba = frames[0].at<cv::Vec4b>(coord.getX(), coord.getY());
                    }
                    else {
                        // For now no noise. Later if the video is properly converted
                        // noise will be inserted here.
                        rgba[0] = 0;
                        rgba[1] = 0;
                        rgba[2] = 0;
                        rgba[3] = 0;
                    }
                }
                else {
                    rgba = frames[coord.getCubeFace()].at<cv::Vec4b>(coord.getX(), coord.getY());
                }
            }
        }
        // Write the output file
        // For now this stay commented
        // cv::imwrite( "path/image.png", output);

        cv::namedWindow( "Test image", cv::CV_WINDOW_AUTOSIZE);
        cv::imshow( "Test image", output);
        cv::waitKey(0);
    }
}