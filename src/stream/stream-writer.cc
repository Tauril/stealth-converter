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
      int nb_video = 0;
      cv::VideoCapture video(StreamData::Instance().video_name_get());

      for (const auto& iframes :
           reader::StreamReader::Instance().iframes_split_get())
      {
        cv::VideoWriter vwriter;
        vwriter.open("__stealth_reader_output_"
                     + std::to_string(nb_video) + ".mp4",
                     0x00000021,
                     StreamData::Instance().fps_get(),
                     cv::Size(video.get(CV_CAP_PROP_FRAME_WIDTH),
                              video.get(CV_CAP_PROP_FRAME_HEIGHT)));

        if (!vwriter.isOpened())
        {
          std::cerr << "Fail to open writer" << std::endl;
          std::exit(1);
        }

        int nb_frame = iframes.first;

        std::cout << "Processing frames [" << iframes.first
                  << ", " << iframes.second << "]" << std::endl;

        for (; nb_frame <= iframes.second; nb_frame++)
        {
          cv::Mat frame;
          video >> frame;
          vwriter << frame;

          frame.release();
        }

        nb_video++;

        vwriter.release();
      }

      video.release();
    }

  } // namespace writer

} // namespace stream
