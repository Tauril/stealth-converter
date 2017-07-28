#include "convert.hh"

namespace convert
{

    Converter::Converter(unsigned int pxInW, double rdInV, double rdInH)
        : Converter(rdInH / constants::m_pi2 * pxInW,
                    rdInV / constants::m_pi2 * pxInW, pxInW,
                    rdInV, rdInH)
    {}

    Converter::Converter(unsigned int pxPanoH, unsigned int pxPanoV,
                         unsigned int pxInW, double rdInV, double rdInH)
        :   pxCamV_(pxInW),
            pxCamH_(pxInW),
            radius_(pxInW / 2.0),
            rdPanoV_(rdInV),
            rdPanoH_(rdInH),
            pxPanoSizeV_(pxPanoV),
            pxPanoSizeH_(pxPanoH),
            cubeFaceId_(CubeFace::front),
            mappedX_(0.0),
            mappedY_(0.0),
            normTheta_(0.0),
            // the actual calculation resolution is 10 times bigger
            // than the texture resolution
            resCal_(constants::m_pi4 / (pxInW * 10.0)),
            // the normalisation factors
            normFactorX_(rdInH / (constants::m_pi * 2)),
            normFactorY_(rdInV / constants::m_pi),
            sizeRatio_(1.0),
            tX_(0.0),
            tY_(0.0),
            tZ_(0.0),
            tTheta_(0.0),
            tPhi_(0.0),
            phiThreshold_(0.0)
    {
        map_ = std::vector<CubeCoordinate*>();

        for (unsigned int x = 0; x < pxPanoSizeH_; ++x) {
            for (unsigned int y = 0; y < pxPanoSizeV_; ++y) {
                computePos(x, y);
                map_.push_back(new CubeCoordinate(cubeFaceId_, mappedX_, mappedY_));
            }
        }
    }

    Converter::~Converter()
    {
        for (auto i : map_) {
            delete i;
        }
    }

}
