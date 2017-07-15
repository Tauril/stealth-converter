#include "stream/stream-reader.hh"
#include "stream/stream-writer.hh"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "usage: stealth-converter <Video_Path>" << std::endl;
    return 1;
  }

  stream::reader::StreamReader::Instance(argv[1]);
  stream::reader::StreamReader::Instance().open_input_file();
  stream::reader::StreamReader::Instance().get_iframes();

  stream::writer::StreamWriter::Instance().split_video();

  return 0;
}
