#pragma once

#include "stream-reader.hh"

namespace stream
{

  namespace reader
  {

    inline
    const std::vector<std::pair<int, int>>&
    StreamReader::iframes_split_get() const
    {
      return iframes_split_;
    }

    inline
    std::map<int, int>
    StreamReader::iframes_pos_get()
    {
      return iframes_pos_;
    }

  } // namespace reader

} // namespace stream
