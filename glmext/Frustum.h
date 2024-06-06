#pragma once

namespace glm
{

    /**
     * This class defines a View frustum Volume as a set of 6 planes
     *
     * @ingroup Types
     */
    template<typename T>
    class frustum_t
    {
    public:
        typedef T DataType;
        typedef frustum_t<T> frustumType;

        /**
         * An enum to name the plane indicies.
         * To have you not must remember those numbers.
         */
        enum PlaneNames
        {
            PLANE_LEFT = 0,      /**< left   clipping plane equals 0 */
            PLANE_RIGHT = 1,   /**< right   clipping plane equals 1 */
            PLANE_BOTTOM = 2,   /**< bottom   clipping plane equals 2 */
            PLANE_TOP = 3,      /**< top   clipping plane equals 3 */
            PLANE_NEAR = 4,      /**< near   clipping plane equals 4 */
            PLANE_FAR = 5      /**< far   clipping plane equals 5 */
        };

        /**
         * Constructs a new frustum with all planes in default state.
         */
        frustum_t()
        {

        }

        /**
         * Constructs a new frustum with the given projection matrix.
         *
         * @param projMatrix The projection matrix of your camera or light etc. to
         *                   construct the planes from.
         */
        frustum_t(const glm::mat<4, 4, T>& projMatrix)
        {
            extractPlanes(projMatrix);
        }

        /**
         * Constructs a new frustum with given projection and modelview matricies.
         * the matricies are multiplied in this order:
         *
         * \code
         * M = projMatrix * modelviewMatrix
         * \endcode
         *
         * The planes are then extracted from M.
         *
         * @param modelviewMatrix The modelview matrix of you camera or light etc.
         *                        to construct the planes from.
         * @param projMatrix      The projection matrix of your camera or light or
         *                        whatever.
         */
        frustum_t(const glm::mat<4, 4, T>& modelviewMatrix,
            const glm::mat<4, 4, T>& projMatrix)
        {
            extractPlanes(modelviewMatrix, projMatrix);
        }

        /**
         * Extracts the planes from the given projection matrix.
         *
         * @param projMatrix The projection matrix of you camera or light or
         *                   what ever.
         */
        void extractPlanes(const glm::mat<4, 4, T>& modelviewMatrix,
            const glm::mat<4, 4, T>& projMatrix)
        {
            extractPlanes(projMatrix * modelviewMatrix);
        }

        /**
         * Extracts the planes from the given projection and modelview matricies.
         * The matricies are multiplied in this order:
         *
         * \code
         * M = projMatrix * modelviewMatrix
         * \endcode
         *
         * The planes are then extracted from M.
         *
         * @param modelviewMatrix The modelview matrix of you camera or light etc.
         *                        to construct the planes from.
         * @param projMatrix      The projection matrix of you camera or light etc.
         *                        to construct the planes from.
         */
        void extractPlanes2(const glm::mat<4, 4, T>& projMatrix)
        {
            const glm::mat<4, 4, T>& m = projMatrix;


            //left
            mPlanes[PLANE_LEFT].setNormal(glm::vec<3, T>(m[0][3] + m[0][0],
                m[1][3] + m[1][0],
                m[2][3] + m[2][0]));
            mPlanes[PLANE_LEFT].setOffset(m[3][3] + m[3][0]);
            //right
            mPlanes[PLANE_RIGHT].setNormal(glm::vec<3, T>(m[0][3] - m[0][0],
                m[1][3] - m[1][0],
                m[2][3] - m[2][0]));
            mPlanes[PLANE_RIGHT].setOffset(m[3][3] - m[3][0]);
            //bottom
            mPlanes[PLANE_BOTTOM].setNormal(glm::vec<3, T>(m[0][3] + m[0][1],
                m[1][3] + m[1][1],
                m[2][3] + m[2][1]));
            mPlanes[PLANE_BOTTOM].setOffset(m[3][3] + m[3][1]);
            //top
            mPlanes[PLANE_TOP].setNormal(glm::vec<3, T>(m[0][3] - m[0][1],
                m[1][3] - m[1][1],
                m[2][3] - m[2][1]));
            mPlanes[PLANE_TOP].setOffset(m[3][3] - m[3][1]);
            //near
            mPlanes[PLANE_NEAR].setNormal(glm::vec<3, T>(m[0][3] + m[0][2],
                m[1][3] + m[1][2],
                m[2][3] + m[2][2]));
            mPlanes[PLANE_NEAR].setOffset(m[3][2] + m[3][3]);
            //far
            mPlanes[PLANE_FAR].setNormal(glm::vec<3, T>(m[0][3] - m[0][2],
                m[1][3] - m[1][2],
                m[2][3] - m[2][2]));
            mPlanes[PLANE_FAR].setOffset(m[3][3] - m[3][2]);

            normalize();
        }



        void extractPlanes(const glm::mat<4, 4, T>& viewProjectionMatrix) {
            glm::mat<4, 4, T> inv = glm::inverse(viewProjectionMatrix);

            glm::vec<3, T> pts[8] =
            {
                glm::vec<3, T>(-1, -1, 0),
                glm::vec<3, T>(-1, 1, 0),
                glm::vec<3, T>(1, -1, 0),
                glm::vec<3, T>(1, 1, 0),
                glm::vec<3, T>(-1, -1, 1),
                glm::vec<3, T>(-1, 1, 1),
                glm::vec<3, T>(1, -1, 1),
                glm::vec<3, T>(1, 1, 1)
            };
            int planes[6][4] =
            {
                { 4, 5, 1, 0 }, // LEFT
                { 2, 3, 7, 6 }, // RIGHT
                { 0, 2, 6, 4 }, // TOP
                { 1, 5, 7, 3 }, // BOTTOM
                { 1, 3, 2, 0 }, // FRONT
                { 5, 4, 6, 7 }, // BOTTOM
            };

            glm::vec<4, T> wpts[8];
            for (int i = 0; i < 8; ++i)
            {
                wpts[i] = inv * glm::vec<4, T>(pts[i], 1);
                wpts[i] /= wpts[i].w;
            }

            for (int i = 0; i < 6; ++i)
            {
                const int* indx = planes[i];
                glm::vec<3, T> vec1 = wpts[indx[1]] - wpts[indx[0]];
                glm::vec<3, T> vec2 = wpts[indx[2]] - wpts[indx[1]];
                glm::vec<3, T> nrm = glm::normalize(glm::cross(vec2, vec1));
                mPlanes[i].mNorm = nrm;
                T dist = glm::dot(nrm, glm::vec<3, T>(wpts[indx[1]]));
                mPlanes[i].mOffset = dist;
            }

        }

        void normalize()
        {
            for (unsigned int i = 0; i < 6; ++i)
            {
                glm::vec<3, T> n = mPlanes[i].getNormal();
                T o = mPlanes[i].getOffset();
                T len = glm::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
                n[0] /= len;
                n[1] /= len;
                n[2] /= len;
                o /= len;
                mPlanes[i].setNormal(n);
                mPlanes[i].setOffset(o);
            }
        }

        glm::plane_t<T> mPlanes[6];
    };

    typedef frustum_t<float> frustumf;
    typedef frustum_t<double> frustumd;

}
