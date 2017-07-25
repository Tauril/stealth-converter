#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "frame-handler.hh"
#include "helper.hh"

namespace convert
{
    void FrameHandler::process(std::vector<std::string> inputs)
    {
        assert(inputs.size() == 1 || inputs.size() == 6);

        std::vector<cv::Mat> frames;
        size_t width = 0;
        // Read the 6 input images
        for (auto img : inputs) {
            cv::Mat image = cv::imread(img, CV_LOAD_IMAGE_COLOR);
            if (!image.data) {
                printf( " No image data \n " );
                return;
            }
            // for now assume the pictures are squares
            assert(image.rows == image.cols);
            width = image.rows;
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

        cv::Mat output(cvrt.getPanoSizeV() , cvrt.getPanoSizeH(), CV_8UC3);

        // Map the pixels from the panorama back to the source image
        for (size_t i = 0; i < cvrt.getPanoSizeH(); ++i) {
            for (size_t j = 0; j < cvrt.getPanoSizeV(); ++j) {
                // Get the corresponding position of (i, j)
                auto coord = cvrt.getCoordinate(i, j);
                // output pixel
                cv::Vec3b& rgba = output.at<cv::Vec3b>(j, i);

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
                    if (x_coord > 640 || y_coord > 640)
                        std::cout << "y: " << y_coord << " x: " << x_coord << std::endl; 
                    rgba = toto.at<cv::Vec3b>(x_coord, y_coord);
                }
            }
        }
        // Write the output file
        // For now this stay commented
        // cv::imwrite( "path/image.png", output);

        cv::namedWindow( "Test image", CV_WINDOW_AUTOSIZE);
        cv::imshow( "Test image", output);
        cv::waitKey(0);
        imwrite( "marques.png", output);
    }
}