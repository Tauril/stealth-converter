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
      const auto& iframes_p =
        reader::StreamReader::Instance().iframes_split_get();
      auto iframes_pos =
        reader::StreamReader::Instance().iframes_pos_get();

#ifdef PARALLEL
      tbb::parallel_for_each(iframes_p.begin(), iframes_p.end(),
        [&](const auto& iframes)
#else
        for (const auto& iframes : iframes_p)
#endif
        {
          cv::VideoCapture video(StreamData::Instance().video_name_get());
          int nb_frame = iframes.first;
          int video_number = iframes_pos[nb_frame];

          if (!video.set(CV_CAP_PROP_POS_FRAMES, nb_frame))
            std::cerr << "Couldn't set next frame" << std::endl;

          cv::VideoWriter vwriter;
          auto video_name =
            "__stealth_reader_output_" + std::to_string(video_number) + ".mp4";
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
        }
#ifdef PARALLEL
      );
#endif
    }

  } // namespace writer

} // namespace stream
