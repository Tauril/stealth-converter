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
            video_prefix_ + std::to_string(video_number) + video_suffix_;
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

    void
    StreamWriter::construct_video(
                        const std::vector<std::string>& sub_videos) const
    {
      cv::VideoWriter vwriter;
      auto video_name = video_prefix_ + video_suffix_;
      // Awful.
      cv::VideoCapture video_tmp(sub_videos[0]);
      vwriter.open(video_name, 0x00000021,
                   video_tmp.get(CV_CAP_PROP_FPS),
                   cv::Size(video_tmp.get(CV_CAP_PROP_FRAME_WIDTH),
                            video_tmp.get(CV_CAP_PROP_FRAME_HEIGHT)));

      if (!vwriter.isOpened())
      {
        std::cerr << "Fail to open writer" << std::endl;
        std::exit(1);
      }

      video_tmp.release();
      for (const auto& video_name : sub_videos)
      {
        cv::VideoCapture video(video_name);
        int nb_frame = video.get(CV_CAP_PROP_FRAME_COUNT);

        for (int j = 0; j < nb_frame; j++)
        {
          cv::Mat frame;

          video >> frame;
          vwriter << frame;

          frame.release();
        }

        video.release();
      }
      vwriter.release();
    }

  } // namespace writer

} // namespace stream
