#include <iostream>

#include "stream-reader/stream-reader.hh"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "usage: stealth-converter <Video_Path>" << std::endl;
    return 1;
  }

  stream_reader::StreamReader stream_reader(argv[1]);

  if (!stream_reader.video_get().isOpened())
  {
    std::cerr << "Could not open input video" << std::endl;
    return 2;
  }

  stream_reader.read();

  return 0;
}
