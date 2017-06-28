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
    using namespace cv;

    for (;;)
    {
      Mat frame;
      video_ >> frame; // get a new frame from camera

      if (frame.empty())
        break;

      //cvtColor(frame, edges);
      imshow(video_name_, frame);
      if (waitKey(25) >= 1000) break;
    }
  }

} // namespace stream_reader
