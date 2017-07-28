#pragma once

namespace convert
{
    enum class CubeFace
    {
        top = 0,
        left = 1,
        front = 2,
        right = 3,
        back = 4,
        down = 5
    };

    class CubeCoordinate
    {
    public:
        CubeCoordinate(CubeFace face, double x, double y);
        inline CubeFace getCubeFace() const;
        inline double getX() const;
        inline double getY() const;

    private:
        CubeFace face;
        double x;
        double y;
    };
}

#include "cube-coordinate.hxx"