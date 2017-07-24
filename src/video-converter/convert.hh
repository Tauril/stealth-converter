#pragma once

#include <vector>
#include "cube-coordinate.hh"

namespace convert
{
    namespace constants
    {
        const double m_pi = 3.14159265358979323846;
        const double m_pi2 = 1.57079632679489661923;
        const double m_pi4 = 0.78539816339744830962;

        // sphere radius
        const int sphere_radius = 1.0;
    }

    // At the moment the main function of the converter (getCoordinate) returns
    // the adequate position in the cubemap from which the pixel should be
    // taken given the coordinate x y in the equirectangle projection
    class Converter
    {
        /** General Procedure:
         ** for each pixel of the destination image:
         **     calculate the corresponding unit vector in 3-dimensional space
         **     calculate the x,y coordinate for that vector in the source image
         **     sample the source image at that coordinate and assign
         ** the value to the destination pixel
         */
    public:

        /** \brief This constructor takes fewer parameters, and will
         * generate the width and height of the panorama automatically
         * based on the input
         *
         * \param pxInW const unsigned int      The width of the input image
         * \param rdInV const          double   The radian of the view portion
         * vertically, range [0.01, PI]
         * \param rdInH const          double   The radian of the view portion
         * horizontally, range [0.01, 2*PI]
         * \return void
         */
        Converter(unsigned int pxInW, double rdInV, double rdInH);


        /** \brief Constructor
         * \param pxPanoH const unsigned int  The desired panorama width
         * \param pxPanoV const unsigned int  The desired panorama height
         * \param pxInW   const unsigned int  The width of the input image
         * \param rdInV   const double        The radian of the view portion
         * vertically, range [0.01, PI]
         * \param rdInH   const double        The radian of the view portion
         * horizontally, range [0.01, 2*PI]
         *
         */
        Converter(unsigned int pxPanoH, unsigned int pxPanoV,
                  unsigned int pxInW, double rdInV, double rdInH);

        ~Converter();

        const CubeCoordinate* getCoordinate(const unsigned int x,
                                            const unsigned int y) const;

        unsigned int getPanoSizeV() const;
        unsigned int getPanoSizeH() const;

    private:
        /**< The map from panorama coordinates to cubic coordinates */
        std::vector<CubeCoordinate*> map;

        unsigned int pxCamV;    /**< The vertical pixel of a camera */
        unsigned int pxCamH;    /**< The horizontal pixel of a camera */

        double radius;          /**< The radius of the sphere */

        double rdPanoV;         /**< The vertical view portion */
        double rdPanoH;         /**< The horizontal view portion */

        //-------- output information
        unsigned int pxPanoSizeV;   /**< The vertical pixels of the panorama */
        unsigned int pxPanoSizeH; /**< The horizontal pixels of the panorama */

        //-------- to access the pixel
        CubeFace    cubeFaceId; /**< The cube face to be read */
        double      mappedX;    /**< The x coordinate mapped on the cube face */
        double      mappedY;    /**< The y coordinate mapped on the cube face */

        //-------- The temp variables TODO: DELETE AS MUCH AS POSSIBLE
        double normTheta;   /**< The normalised theta */
        double resCal;      /**< The resolution used for calculation */
        double normFactorX; /**< The normalisation factor for x */
        double normFactorY; /**< The normalisation factor for y */

        /**< The size ratio of the mapped x and the actual radius */
        double sizeRatio;

        double tX;          /**< x coordinate in 3D space */
        double tY;          /**< y coordinate in 3D space */
        double tZ;          /**< z coordinate in 3D space */

        double tTheta;      /**< The radian horizontally */
        double tPhi;        /**< The radian vertically */

        /**< The threshold of phi, it separates the top, middle and down
        of the cube, which are the edges of the top and down surface */
        double phiThreshold;

        /** \brief Calculate the x and y coordinates of given i and j
         *
         * \param i const int&  The coordinate along the width axis
         * \param j const int&  The coordinate along the height axis
         * \return void
         */
        inline void computePos(const int& i, const int& j);

        /** \brief Calculate the face which the point is on
         *
         * \param theta const double&
         * \param phi const double&
         * \return void
         */
        inline void calCubeFace(const double& theta, const double& phi);

        /** \brief Locate the point in the cubic image
         *
         * \param axis const double& The sphere coordinate along the axis
         * \param px   const double&
         * \param py   const double&
         * \param rad  const double&
         * \return void
         */
        inline void locate(const double& axis, const double& px,
                           const double& py, const double& rad);

        /** Test if double value in in the range of [small, large)
            TODO: move to helper class
        */
        inline bool isDoubleInRange(const double& value, const double& small,
                                    const double& large, const double& epsilon);
    };
}
