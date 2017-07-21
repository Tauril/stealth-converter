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
            // the actual calculation resolution is 10 times bigger
            // than the texture resolution
            resCal(constants::m_pi4 / (pxInW * 10.0)),
            // the normalisation factors
            normFactorX(rdInH / (constants::m_pi * 2)),
            normFactorY(rdInV / constants::m_pi)
    {
        map = std::vector<CubeCoordinate*>();
        unsigned int pos = 0;

        for (unsigned int x = 0; x < pxPanoSizeH; ++x) {
            for (unsigned int y = 0; y < pxPanoSizeV; ++y, ++pos) {
                calXY(x, y);
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

    inline void Converter::calXY(const int& i, const int& j)
    {
        // FIXME
    }

}
