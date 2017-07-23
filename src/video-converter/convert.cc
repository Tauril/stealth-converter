#include <cmath>
#include "convert.hh"

namespace convert
{

    Converter::Converter(unsigned int pxInW, double rdInV, double rdInH)
        : Converter(rdInH / constants::m_pi2 * pxInW,
                    rdInH / constants::m_pi2 * rdInV, pxInW,
                    rdInV, rdInH)
    {}

    Converter::Converter(unsigned int pxPanoH, unsigned int pxPanoV,
                         unsigned int pxInW, double rdInV, double rdInH)
        :   pxCamV(pxInW),
            pxCamH(pxInW),
            radius(pxInW / 2.0),
            rdPanoV(rdInV),
            rdPanoH(rdInH),
            pxPanoSizeV(pxPanoV),
            pxPanoSizeH(pxPanoH),
            cubeFaceId(CubeFace::front),
            mappedX(0.0),
            mappedY(0.0),
            normTheta(0.0),
            // the actual calculation resolution is 10 times bigger
            // than the texture resolution
            resCal(constants::m_pi4 / (pxInW * 10.0)),
            // the normalisation factors
            normFactorX(rdInH / (constants::m_pi * 2)),
            normFactorY(rdInV / constants::m_pi),
            sizeRatio(1.0),
            tX(0.0),
            tY(0.0),
            tZ(0.0),
            tTheta(0.0),
            tPhi(0.0),
            phiThreshold(0.0)
    {
        map = std::vector<CubeCoordinate*>();
        unsigned int pos = 0;

        for (unsigned int x = 0; x < pxPanoSizeH; ++x) {
            for (unsigned int y = 0; y < pxPanoSizeV; ++y, ++pos) {
                computePos(x, y);
                map[pos] = new CubeCoordinate(cubeFaceId, mappedX, mappedY);
            }
        }
    }

    Converter::~Converter()
    {
        for (auto i : map) {
            delete i;
        }
    }

    const CubeCoordinate* Converter::getCoordinate(const unsigned int x,
                                                   const unsigned int y) const
    {
        return map[x * pxPanoSizeV + y];
    }

    inline void Converter::computePos(const int& i, const int& j)
    {
        tX = ((2.0 * i) / pxPanoSizeH - 1.0) * normFactorX;
        tY = ((2.0 * j) / pxPanoSizeV - 1.0) * normFactorY;
        tTheta = tX * constants::m_pi;
        tPhi = tY * constants::m_pi2;
        tX = std::cos(tPhi) * std::cos(tTheta);
        tY = std::sin(tPhi);
        tZ = std::cos(tPhi) * std::sin(tTheta);

        calCubeFace(tTheta, tPhi);

        switch (cubeFaceId) {
        case CubeFace::top :
            locate(tY, tZ, tX, constants::m_pi);
            break;
        case CubeFace::down :
            locate(tY, tX, tZ, -constants::m_pi2);
            break;
        case CubeFace::left :
            locate(tZ, tX, tY, constants::m_pi);
            break;
        case CubeFace::right:
            locate(tZ, tY, tX, constants::m_pi2);
            break;
        case CubeFace::front :
            locate(tX, tZ, tY, 0.0);
            break;
        case CubeFace::back :
            locate(tX, tY, tZ, -constants::m_pi2);
            break;
        default:
            break;
        }
    }

    inline void Converter::calCubeFace(const double& theta, const double& phi) {
        // Looking at the cube from top down
        // FRONT zone
        if (isDoubleInRange(theta, -constants::m_pi4,
                            constants::m_pi4, resCal)) {
            cubeFaceId = CubeFace::front;
            normTheta  = theta;
        }
        // LEFT zone
        else if (isDoubleInRange(theta, -(constants::m_pi2 + constants::m_pi4),
                                 -constants::m_pi4, resCal)) {
            cubeFaceId = CubeFace::left;
            normTheta  = theta + constants::m_pi2;
        }
        // RIGHT zone
        else if (isDoubleInRange(theta, constants::m_pi4,
                 constants::m_pi2 + constants::m_pi4, resCal)) {
            cubeFaceId = CubeFace::right;
            normTheta  = theta - constants::m_pi2;
        }
        else {
            cubeFaceId = CubeFace::back;
            normTheta = theta + constants::m_pi * ((theta > 0.0) ? -1.0 : 1.0);
        }
        // find out which segment the line strikes to
        phiThreshold = std::atan2(constants::sphere_radius,
                                  constants::sphere_radius / cos(normTheta));
        // in the top segment
        if (phi > phiThreshold) {
            cubeFaceId = CubeFace::down;
        }
        // in the bottom segment
        else if (phi < -phiThreshold) {
            cubeFaceId = CubeFace::top;
        }
    }

    // Checks the range for double ought to be in a helper class
    inline bool
    Converter::isDoubleInRange(const double& value, const double& small,
                               const double& large, const double& epsilon) {
        bool equalSup = ((value - small) > 0) || (std::fabs(value - small) < epsilon);
        bool smaller = ((value - large) < 0) && !(std::fabs(value - large) < epsilon);
        return equalSup && smaller;
    }

    inline void
    Converter::locate(const double& axis, const double& px,
                      const double& py, const double& rad) {
        this->sizeRatio = radius / axis;
        this->mappedX = sizeRatio * px;
        this->mappedY = sizeRatio * py;
        // Rotate the point for the given radian rad
        sizeRatio = mappedX;
        mappedX = mappedX * std::cos(rad) - mappedY * std::sin(rad);
        mappedY = sizeRatio * std::sin(rad) + mappedY * std::cos(rad);
        // Translate the point for a given distance in both x and y coordinates
        mappedX += radius;
        mappedY += radius;
    }

}
