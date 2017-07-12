#include "stream-reader.hh"

namespace stream_reader
{

  StreamReader::StreamReader(const std::string& video)
    : video_(video)
  {};

  StreamReader::~StreamReader()
  {
    avfilter_graph_free(&filter_graph_);
    if (codec_ctx_)
      avcodec_close(codec_ctx_);
    avformat_close_input(&format_ctx_);
  };

  StreamReader&
  StreamReader::Instance(const std::string& video)
  {
    static StreamReader instance_(video);
    return instance_;
  }

  void
  StreamReader::open_input_file()
  {
    av_register_all();
    avfilter_register_all();

    AVCodec* cdec;

    int ret =
      avformat_open_input(&format_ctx_, video_.c_str(), nullptr, nullptr);
    if (ret < 0)
    {
      std::cerr << "Failed to open input file" << std::endl;
      std::exit(ret);
    }

    if ((ret = avformat_find_stream_info(format_ctx_, nullptr)) < 0)
    {
      std::cerr << "Cannot find stream information" << std::endl;
      std::exit(ret);
    }

    ret =
      av_find_best_stream(format_ctx_, AVMEDIA_TYPE_VIDEO, -1, -1, &cdec, 0);
    if (ret < 0)
    {
      std::cerr << "Cannot find a video stream in the input file" << std::endl;
      std::exit(ret);
    }

    vstream_idx_ = ret;
    codec_ctx_ = format_ctx_->streams[vstream_idx_]->codec;

    if ((ret = avcodec_open2(codec_ctx_, cdec, nullptr)) < 0)
    {
      std::cerr << "Cannot open video decoder" << std::endl;
      std::exit(ret);
    }

    init_filter_graph();
  }

  void
  StreamReader::init_filter_graph()
  {
    auto buffer_src   = avfilter_get_by_name("buffer");
    auto buffer_sink  = avfilter_get_by_name("buffersink");
    auto outputs      = avfilter_inout_alloc();
    auto inputs       = avfilter_inout_alloc();

    filter_graph_ = avfilter_graph_alloc();

    char args[512];

    std::snprintf(args, sizeof (args),
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
             codec_ctx_->width, codec_ctx_->height, codec_ctx_->pix_fmt,
             codec_ctx_->time_base.num, codec_ctx_->time_base.den,
             codec_ctx_->sample_aspect_ratio.num,
             codec_ctx_->sample_aspect_ratio.den);

    int ret =
      avfilter_graph_create_filter(&buffer_src_ctx_, buffer_src, "in",
                                   args, nullptr, filter_graph_);
    if (ret < 0)
    {
      std::cerr << "Cannot create buffer source" << std::endl;
      std::exit(ret);
    }

    auto buffer_sink_params = av_buffersink_params_alloc();
    AVPixelFormat pix_fmts[] = { AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE };

    buffer_sink_params->pixel_fmts = pix_fmts;
    ret =
      avfilter_graph_create_filter(&buffer_sink_ctx_, buffer_sink, "out",
                                   nullptr, buffer_sink_params, filter_graph_);
    av_free(buffer_sink_params);

    if (ret < 0)
    {
      std::cerr << "Cannot create buffer sink" << std::endl;
      std::exit(ret);
    }

    inputs->name        = av_strdup("out");
    inputs->filter_ctx  = buffer_sink_ctx_;
    inputs->pad_idx     = 0;
    inputs->next        = nullptr;

    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffer_src_ctx_;
    outputs->pad_idx    = 0;
    outputs->next       = nullptr;

    ret = avfilter_graph_parse_ptr(filter_graph_, filter_, &inputs,
                                      &outputs, nullptr);
    if (ret < 0)
      std::exit(ret);

    if ((ret = avfilter_graph_config(filter_graph_, nullptr)) < 0)
      std::exit(ret);
  }

  void
  StreamReader::get_iframes()
  {
    int has_frame_p = 0;
    int nb_frame = 0;

    auto frame_del = [](AVFrame* frame) { av_frame_free(&frame); };

    AVPacket packet;
    std::unique_ptr<AVFrame, decltype(frame_del)>
      frame(av_frame_alloc(), frame_del);
    std::unique_ptr<AVFrame, decltype(frame_del)>
      frame_out(av_frame_alloc(), frame_del);

    while (1)
    {
      int ret = 0;

      if ((ret = av_read_frame(format_ctx_, &packet)) < 0)
        break;

      if (packet.stream_index == vstream_idx_)
      {
        has_frame_p = 0;
        avcodec_decode_video2(codec_ctx_, frame.get(), &has_frame_p, &packet);
        if (ret < 0)
        {
          std::cerr << "Error decoding video" << std::endl;
          break;
        }

        if (has_frame_p)
        {
          frame->pts = av_frame_get_best_effort_timestamp(frame.get());

          if (av_buffersrc_add_frame(buffer_src_ctx_, frame.get()) < 0)
          {
            std::cerr << "Error while feeding the filter graph" << std::endl;
            break;
          }

          while (1)
          {
            ret = av_buffersink_get_frame(buffer_sink_ctx_, frame_out.get());
            if (ret < 0)
              break;

            std::cout << "Process frame: " << ++nb_frame << std::endl;

            // Private scope just to display frame for debugging purpose.
            {
              cv::Mat m;
              AVFrame dst;
              int w = frame_out->width;
              int h = frame_out->height;
              m = cv::Mat(h, w, CV_8UC3);
              dst.data[0] = m.data;
              avpicture_fill(reinterpret_cast<AVPicture*>(&dst), dst.data[0],
                             PIX_FMT_BGR24, w, h);

              auto src_pix_fmt =
                static_cast<PixelFormat>(frame_out->format);

              auto sws_ctx = sws_getContext(w, h, src_pix_fmt, w, h,
                                            PIX_FMT_BGR24, SWS_FAST_BILINEAR,
                                            nullptr, nullptr, nullptr);

              sws_scale(sws_ctx, frame_out->data, frame_out->linesize, 0, h,
                        dst.data, dst.linesize);

              cv::imshow(video_, m);

              if (cv::waitKey(100) >= 1000)
                break;
            }
            av_frame_unref(frame_out.get());
          }
        }
        av_frame_unref(frame.get());
      }
      av_free_packet(&packet);
    }
  }

} // namespace stream_reader
