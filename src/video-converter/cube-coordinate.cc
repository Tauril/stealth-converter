#include "cube-coordinate.hh"

namespace convert
{
    CubeCoordinate::CubeCoordinate(CubeFace face, double x, double y)
    {
        this->face = face;
        this->x = x;
        this->y = y;
    }
    
    CubeFace CubeCoordinate::getCubeFace() const
    {
        return this->face;
    }
    
    double CubeCoordinate::getX() const
    {
        return this->x;
    }
    
    double CubeCoordinate::getY() const
    {
        return this->y;
    }
}