#include "stream-writer.hh"

namespace stream
{

  namespace writer
  {

    StreamWriter&
    StreamWriter::Instance()
    {
      static StreamWriter instance;
      return instance;
    }

    void
    StreamWriter::split_video() const
    {
      int nb_video = -1;

      const auto& iframes_p =
        reader::StreamReader::Instance().iframes_split_get();

      tbb::parallel_for_each(iframes_p.begin(), iframes_p.end(),
        [&](const auto& iframes)
        {
          cv::VideoCapture video(StreamData::Instance().video_name_get());
          int nb_frame = iframes.first;

          if (!video.set(CV_CAP_PROP_POS_FRAMES, nb_frame))
            std::cerr << "Couldn't set next frame" << std::endl;

          cv::VideoWriter vwriter;
          auto video_name =
            "__stealth_reader_output_" + std::to_string(++nb_video) + ".mp4";
          vwriter.open(video_name, 0x00000021,
                       StreamData::Instance().fps_get(),
                       cv::Size(video.get(CV_CAP_PROP_FRAME_WIDTH),
                                video.get(CV_CAP_PROP_FRAME_HEIGHT)));

          if (!vwriter.isOpened())
          {
            std::cerr << "Fail to open writer" << std::endl;
            std::exit(1);
          }


          std::cout << "Processing frames [" << iframes.first
                    << ", " << iframes.second << "]" << std::endl;

          std::cout << video_name << ":"
                    << std::to_string(iframes.second - iframes.first)
                    << std::endl;

          for (; nb_frame <= iframes.second; nb_frame++)
          {
            cv::Mat frame;

            video >> frame;
            vwriter << frame;

            frame.release();
          }

          vwriter.release();
          video.release();
        });
    }

  } // namespace writer

} // namespace stream
