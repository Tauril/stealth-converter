#pragma once

#include <iostream>
#include <memory>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/avfiltergraph.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace stream_reader
{

  class StreamReader
  {
    public:
      StreamReader(const std::string& video);
      ~StreamReader();

      void read();

      void filter_iframes();

      const std::string& video_name_get() const;

    private:
      std::string video_name_;
      AVFormatContext* in_ctx_            = nullptr;
      AVStream* vstream_                  = nullptr;
      int vstream_index_;
      AVFrame* frame_                     = nullptr;
      std::vector<uint8_t> frame_buff_;
      AVFrame* dec_frame_                 = nullptr;
      SwsContext* sws_ctx_                = nullptr;
      int height_;
      int width_;
  };

} // namespace stream_reader

#include "stream-reader.hxx"
