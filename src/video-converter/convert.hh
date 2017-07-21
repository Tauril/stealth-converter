#pragma once

#include <iostream>
#include <cmath>
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

    /** This class represents the main processing step to convert a 2D video
     ** into a "false" 3D video
     ** The main method of this class is .....
     ** The input of this method is a 2D frame and a noise sample. Default
     ** noise sanmple is transparent. (Maybe false #tbd)
     ** The output is an equirectangle frame with a 2:1 aspect ratio
     ** and 7168:3584 or higher resolution (YouTube format)
     **
     ** The aim is to use the input frames and noises as a cubemap and convert
     ** it to equirectangle.
     ** The next step ought to be grouping all the equirectangle frame
     ** into a video with ffmpeg
     */

    /** This class in in need of a Wrapper that will do the following:
     ** 2 Options : Transparent all pixels value at 0 (0, 0, 0, 0)
     ** Noise generated :  rand() to add noise, and conv2() or
     ** imfilter() to blur the image.
     ** + option parallel or not with TBB
     ** compute(picture + Transparent or noise) returns Equirectangle
     */

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

        inline void calXY(const int& i, const int& j);

        inline void calXYZ(const int& i, const int& j, double& x,
                           double& y, double& z);

        inline void calNormXY(const int& i, const int& j, double& x, double& y);
        inline void calThetaAndPhi(const double& x, const double& y,
                                   double& theta, double& phi);
        inline void calXyzFromThetaPhi(const double& theta, const double& phi,
                                       double& x, double& y, double& z);

        inline void calCubeFace(const double& theta, const double& phi);

        inline void locate(const double& axis, const double& px,
                           const double& py, const double& rad);

        // the helper functions
        inline bool cmpDoubleEqual(const double& a, const double& b,
                                   const double& epsilon);
        inline bool cmpDoubleSmaller(const double& a, const double& b,
                                     const double& epsilon);
        inline bool cmpDoubleEqualSmaller(const double& a, const double& b,
                                          const double& epsilon);
        inline bool cmpDoubleLarger(const double& a, const double& b,
                                    const double& epsilon);
        inline bool cmpDoubleEqualLarger(const double& a, const double& b,
                                         const double& epsilon);
        inline bool isDoubleInRange(const double& value, const double& small,
                                    const double& large, const double& epsilon);

        inline void rotRad(double rad, double& x, double& y, double& temp);
        inline void transDis(double dis, double& x, double& y);
    };
}
