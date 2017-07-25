#pragma once

namespace help
{
    template <typename Enumeration>
    auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type;

}

#include "helper.hxx"