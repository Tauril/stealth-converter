#pragma once

namespace convert
{
    enum class CubeFace
    {
        top,
        left,
        front,
        right,
        back,
        down
    };

    class CubeCoordinate
    {
    public:
        CubeCoordinate(CubeFace face, double x, double y);
        CubeFace getCubeFace() const;
        double getX() const;
        double getY() const;

    private:        
        CubeFace face;
        double x;
        double y;
    };

}