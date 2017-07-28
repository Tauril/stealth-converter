
namespace convert
{
    inline CubeFace CubeCoordinate::getCubeFace() const
    {
        return this->face;
    }

    inline double CubeCoordinate::getX() const
    {
        return this->x;
    }

    inline double CubeCoordinate::getY() const
    {
        return this->y;
    }
}