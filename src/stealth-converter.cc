#include "stream-reader/stream-reader.hh"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "usage: stealth-converter <Video_Path>" << std::endl;
    return 1;
  }

  stream_reader::StreamReader::Instance(argv[1]);
  stream_reader::StreamReader::Instance().open_input_file();
  stream_reader::StreamReader::Instance().get_iframes();

  return 0;
}
