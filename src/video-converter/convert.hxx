#include <cmath>
#include "helper.hh"

namespace convert
{
    inline unsigned int Converter::getPanoSizeV() const
    {
        return this->pxPanoSizeV_;
    }

    inline unsigned int Converter::getPanoSizeH() const
    {
        return this->pxPanoSizeH_;
    }

    inline const CubeCoordinate* Converter::getCoordinate(const unsigned int x,
                                                   const unsigned int y) const
    {
        return map_[x * pxPanoSizeV_ + y];
    }

    inline void Converter::computePos(const int& i, const int& j)
    {
        tX_ = ((2.0 * i) / pxPanoSizeH_ - 1.0) * normFactorX_;
        tY_ = ((2.0 * j) / pxPanoSizeV_ - 1.0) * normFactorY_;
        tTheta_ = tX_ * constants::m_pi;
        tPhi_ = tY_ * constants::m_pi2;
        tX_ = std::cos(tPhi_) * std::cos(tTheta_);
        tY_ = std::sin(tPhi_);
        tZ_ = std::cos(tPhi_) * std::sin(tTheta_);

        calCubeFace(tTheta_, tPhi_);

        switch (cubeFaceId_) {
        case CubeFace::top :
            locate(tY_, tZ_, tX_, constants::m_pi);
            break;
        case CubeFace::down :
            locate(tY_, tX_, tZ_, -constants::m_pi2);
            break;
        case CubeFace::left :
            locate(tZ_, tX_, tY_, constants::m_pi);
            break;
        case CubeFace::right:
            locate(tZ_, tY_, tX_, constants::m_pi2);
            break;
        case CubeFace::front :
            locate(tX_, tZ_, tY_, 0.0);
            break;
        case CubeFace::back :
            locate(tX_, tY_, tZ_, -constants::m_pi2);
            break;
        default:
            break;
        }
    }

    inline void Converter::calCubeFace(const double& theta, const double& phi) {
        // Looking at the cube from top down
        // FRONT zone
        if (help::isDoubleInRange(theta, -constants::m_pi4,
                            constants::m_pi4, resCal_)) {
            cubeFaceId_ = CubeFace::front;
            normTheta_  = theta;
        }
        // LEFT zone
        else if (help::isDoubleInRange(theta, -(constants::m_pi2 + constants::m_pi4),
                                 -constants::m_pi4, resCal_)) {
            cubeFaceId_ = CubeFace::left;
            normTheta_  = theta + constants::m_pi2;
        }
        // RIGHT zone
        else if (help::isDoubleInRange(theta, constants::m_pi4,
                 constants::m_pi2 + constants::m_pi4, resCal_)) {
            cubeFaceId_ = CubeFace::right;
            normTheta_  = theta - constants::m_pi2;
        }
        else {
            cubeFaceId_ = CubeFace::back;
            normTheta_ = theta + constants::m_pi * ((theta > 0.0) ? -1.0 : 1.0);
        }
        // find out which segment the line strikes to
        phiThreshold_ = std::atan2(constants::sphere_radius,
                                  constants::sphere_radius / cos(normTheta_));
        // in the top segment
        if (phi > phiThreshold_) {
            cubeFaceId_ = CubeFace::down;
        }
        // in the bottom segment
        else if (phi < -phiThreshold_) {
            cubeFaceId_ = CubeFace::top;
        }
    }

    inline void
    Converter::locate(const double& axis, const double& px,
                      const double& py, const double& rad) {
        this->sizeRatio_ = radius_ / axis;
        this->mappedX_ = sizeRatio_ * px;
        this->mappedY_ = sizeRatio_ * py;
        // Rotate the point for the given radian rad
        sizeRatio_ = mappedX_;
        mappedX_ = mappedX_ * std::cos(rad) - mappedY_ * std::sin(rad);
        mappedY_ = sizeRatio_ * std::sin(rad) + mappedY_ * std::cos(rad);
        // Translate the point for a given distance in both x and y coordinates
        mappedX_ += radius_;
        mappedY_ += radius_;
    }
}