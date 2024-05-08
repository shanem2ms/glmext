// Added by Shane Morrison

#ifndef _GMTL_UNITS_H_
#define _GMTL_UNITS_H_
#include <glm/gtc/random.hpp>

namespace glm
{
    template<class T>
    class altitude
    {
    public:
        altitude() : mData(0) {}
        altitude(const T &v) :
            mData(v) {}
        operator T() const
        {
            return mData;
        }
        T mData;
    };
    
    namespace Units
    {
        // distance constants.
        constexpr double METERS_PER_INCH = 2.54 / 100.0;
        constexpr double INCHES_PER_FOOT = 12.0;
        constexpr double FEET_PER_MILE = 5280.0;
        constexpr double MetersPerFoot = METERS_PER_INCH * INCHES_PER_FOOT;
        constexpr double FeetPerMeter = 1.0 / MetersPerFoot;
        constexpr double MetersPerMile = METERS_PER_INCH * INCHES_PER_FOOT * FEET_PER_MILE;
        constexpr double MilesPerMeter = 1.0 / MetersPerMile;
        constexpr int SecondsPerHour = 3600;

        // Earth radii.  Distances are in meters.  The mean radius is 6367444.5.
        constexpr double EarthEquatorialRadius = 6378137.0;
        constexpr double EarthPolarRadius = 6356752.0;
        constexpr double EarthMeanRadius = (EarthEquatorialRadius + EarthPolarRadius) / 2.0;
    }

    enum DistanceUnit
    {
        Meters,
        Kilometers,
        Miles,
        Earthians
    };

    template <class T> T distance(glm::vec<3, T>& pt0, glm::vec<3, T>& pt1)
    {
        return glm::length(pt0 - pt1);
    }

    template<class T>
    class distance_t
    {
    public:

        distance_t(T d, DistanceUnit u = DistanceUnit::Meters)
        {
            switch (u)
            {
            case Miles:
                mData = d * Units::MetersPerMile;
                break;
            case Kilometers:
                mData = d * 1000;
                break;
            case Earthians:
                mData = d * Units::EarthMeanRadius;
                break;
            case Meters:
            default:
                mData = d;
                break;
            }
        }

        T As(DistanceUnit u) const
        {
            switch (u)
            {
            case Miles:
                return mData * Units::MilesPerMeter;
            case Kilometers:
                return mData / 1000.0;
            case Earthians:
                return mData / Units::EarthMeanRadius;
            case Meters:
            default:
                return mData;
            }
        }

        operator T() const
        {
            return mData;
        }
        // Meters
        T mData;
    };


    template <typename T> class range
    {
    public:

        range() :
            mMin(1),
            mMax(-1) {}

        range(T mn, T mx) :
            mMin(mn),
            mMax(mx) {}

        bool isvalid() const { return mMax >= mMin; }

        T mMin;
        T mMax;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline uint32_t nextPowerOfTwo(uint32_t num)
    {
        if (num == 0)
            return 1;

        num--;
        num = (num >> 1) | num;
        num = (num >> 2) | num;
        num = (num >> 4) | num;
        num = (num >> 8) | num;
        num = (num >> 16) | num;
        num++;

        return num;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline uint64_t nextPowerOfTwo(uint64_t num)
    {
        if (num == 0)
            return 1;

        num--;
        num = (num >> 1) | num;
        num = (num >> 2) | num;
        num = (num >> 4) | num;
        num = (num >> 8) | num;
        num = (num >> 16) | num;
        num = (num >> 32) | num;
        num++;

        return num;
    }
    

    inline float unitRandom()
    {
        return static_cast<float>(::rand()) / static_cast<float>(RAND_MAX);
    }

    template <typename T> T sqr(const T &val) 
    {
        return val * val;
    }

    template <typename T> T lensq(const glm::vec<3, T>& v)
    {
        return glm::dot(v, v);
    }
    template <typename T> T lensq(const glm::vec<2, T>& v)
    {
        return glm::dot(v, v);
    }
    

    // --- helper types --- //
    typedef distance_t<float> distancef;
    typedef distance_t<double> distanced;
    typedef altitude<float> altitudef;
    typedef altitude<double> altituded;
    //typedef vec<4, uint32_t> uivec4;

    enum VectorIndex { Xelt = 1, Yelt = 2, Zelt = 3, Welt = 0 };
     /** Set the rotation portion of a matrix (3x3) from a rotation quaternion.
    * @pre only 3x3, 3x4, 4x3, or 4x4 matrices are allowed, function is undefined otherwise.
    */
    template <typename T>
    mat<4, 4, T> makeRot(const qua<T>& q)
    {
        mat<4, 4, T> m;
        // From Watt & Watt
        T wx, wy, wz, xx, yy, yz, xy, xz, zz, xs, ys, zs;

        xs = q[Xelt] + q[Xelt]; ys = q[Yelt] + q[Yelt]; zs = q[Zelt] + q[Zelt];
        xx = q[Xelt] * xs;      xy = q[Xelt] * ys;      xz = q[Xelt] * zs;
        yy = q[Yelt] * ys;      yz = q[Yelt] * zs;      zz = q[Zelt] * zs;
        wx = q[Welt] * xs;      wy = q[Welt] * ys;      wz = q[Welt] * zs;

        m[0][0] = T(1.0) - (yy + zz);
        m[0][1] = xy + wz;
        m[0][2] = xz - wy;

        m[1][0] = xy - wz;
        m[1][1] = T(1.0) - (xx + zz);
        m[1][2] = yz + wx;

        m[2][0] = xz + wy;
        m[2][1] = yz - wx;
        m[2][2] = T(1.0) - (xx + yy);

        m[3][3] = 1;
        return m;
    }

    template <typename T> qua<T> QuatFromAA(T angle, const vec<3, T> axis)
    {
        T half_angle = angle * (T)0.5;
        T sin_half_angle = glm::sin(half_angle);

        qua<T> result;
        result[Welt] = glm::cos(half_angle);
        result[Xelt] = sin_half_angle * axis[0];
        result[Yelt] = sin_half_angle * axis[1];
        result[Zelt] = sin_half_angle * axis[2];

        // should automagically be normalized (unit magnitude) now...
        return result;
    }
}
#endif