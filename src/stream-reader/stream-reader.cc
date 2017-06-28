#include "stream-reader.hh"

namespace stream_reader
{

  StreamReader::StreamReader(const std::string& video)
    : video_(video)
    , video_name_(video)
  {};

  void
  StreamReader::run()
  {
    for (;;)
    {
      cv::Mat frame;
      video_ >> frame;

      double timestamp = video_.get(CV_CAP_PROP_POS_MSEC);
      std::cout << timestamp / 1000 << std::endl;

      if (frame.empty())
        break;

      cv::imshow(video_name_, frame);
      //if (cv::waitKey(30) >= 0) break;
    }
  }

} // namespace stream_reader
