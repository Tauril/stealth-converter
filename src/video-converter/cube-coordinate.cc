#include "cube-coordinate.hh"

namespace convert
{
    CubeCoordinate::CubeCoordinate(CubeFace face, double x, double y)
    {
        this->face = face;
        this->x = x;
        this->y = y;
    }
}
