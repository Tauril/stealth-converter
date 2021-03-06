#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "stream/stream-reader.hh"
#include "stream/stream-writer.hh"
#include "video-converter/cube-coordinate.hh"
#include "video-converter/frame-handler.hh"

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

enum class ProgramChoice {
    split,
    convert,
    recombine
};

void help_display() {
    std::cerr << "usage:" << std::endl
        << "\thelp:\t\t"
        << "stealth-converter [-h|--help]"
        << std::endl
        << "\tsplit:\t\t"
        << "stealth-converter [-s|--split] <Video_Path>"
        << std::endl
        << "\tconvert:\t"
        << "stealth-converter [-c|--convert] <Video_Path>"
        << std::endl
        << "\trecombine:\t"
        << "stealth-converter [-r|--recombine] <Sub_videos>"
        << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2 || cmdOptionExists(argv, argv + argc, "-h")
            || cmdOptionExists(argv, argv + argc, "--help")) {
        help_display();
        return 0;
    }
#ifdef PARALLEL
    std::cout << "Parallel option actived\n";
#endif

    ProgramChoice prgm = ProgramChoice::split;
    std::vector<std::string> params;
    for (ssize_t idx = 1; idx < argc; ++idx) {
        if (!strcmp(argv[idx], "-s") || !strcmp(argv[idx], "--split"))
            prgm = ProgramChoice::split;
        else if (!strcmp(argv[idx], "-c") || !strcmp(argv[idx], "--convert"))
            prgm = ProgramChoice::convert;
        else if (!strcmp(argv[idx], "-r") || !strcmp(argv[idx], "--recombine"))
            prgm = ProgramChoice::recombine;
        else
            params.push_back(argv[idx]);
    }

    if (prgm == ProgramChoice::split) {
        if (params.size() != 1)
        {
            help_display();
            return 1;
        }

        stream::reader::StreamReader::Instance(params[0]);
        stream::reader::StreamReader::Instance().open_input_file();
        stream::reader::StreamReader::Instance().get_iframes();
        stream::writer::StreamWriter::Instance().split_video();
    }
    else if (prgm == ProgramChoice::convert) {
        // Order: top, left, front, right, back, down
        auto handler = convert::FrameHandler();
        for (const auto& param : params)
          handler.process_video_file(param);
    }
    else if (prgm == ProgramChoice::recombine)
    {
      stream::writer::StreamWriter::Instance().construct_video(params);
    }

    return 0;
}
