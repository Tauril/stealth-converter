#pragma once

#include <vector>
#include "cube-coordinate.hh"

namespace convert
{
    // The main function of the converter (getCoordinate) returns
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

        inline const CubeCoordinate* getCoordinate(const unsigned int x,
                                            const unsigned int y) const;

        inline unsigned int getPanoSizeV() const;
        inline unsigned int getPanoSizeH() const;

    private:
        /**< The map from panorama coordinates to cubic coordinates */
        std::vector<CubeCoordinate*> map_;

        unsigned int pxCamV_;    /**< The vertical pixel of a camera */
        unsigned int pxCamH_;    /**< The horizontal pixel of a camera */

        double radius_;          /**< The radius of the sphere */

        double rdPanoV_;         /**< The vertical view portion */
        double rdPanoH_;         /**< The horizontal view portion */

        //-------- output information
        unsigned int pxPanoSizeV_;   /**< The vertical pixels of the panorama */
        unsigned int pxPanoSizeH_; /**< The horizontal pixels of the panorama */

        //-------- to access the pixel
        CubeFace    cubeFaceId_; /**< The cube face to be read */
        double      mappedX_;    /**< The x coordinate mapped on the cube face */
        double      mappedY_;    /**< The y coordinate mapped on the cube face */

        //-------- The temp variables TODO: DELETE AS MUCH AS POSSIBLE
        double normTheta_;   /**< The normalised theta */
        double resCal_;      /**< The resolution used for calculation */
        double normFactorX_; /**< The normalisation factor for x */
        double normFactorY_; /**< The normalisation factor for y */

        /**< The size ratio of the mapped x and the actual radius */
        double sizeRatio_;

        double tX_;          /**< x coordinate in 3D space */
        double tY_;          /**< y coordinate in 3D space */
        double tZ_;          /**< z coordinate in 3D space */

        double tTheta_;      /**< The radian horizontally */
        double tPhi_;        /**< The radian vertically */

        /**< The threshold of phi, it separates the top, middle and down
        of the cube, which are the edges of the top and down surface */
        double phiThreshold_;

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
    };
}

#include "convert.hxx"