namespace glm
{
    template<class T>
    class angle_t
    {
    public:
        /// The datatype used for the components of this Vec.
        typedef T DataType;

        static inline float deg2Rad(float fVal)
        {
            return static_cast<float>(fVal * static_cast<float>(glm::pi<float>() / 180.0));
        }
        static inline double deg2Rad(double fVal)
        {
            return static_cast<double>(fVal * static_cast<double>(glm::pi<float>() / 180.0));
        }

        static inline float rad2Deg(float fVal)
        {
            return static_cast<float>(fVal * static_cast<float>(180.0 / glm::pi<float>()));
        }
        static inline double rad2Deg(double fVal)
        {
            return static_cast<float>(fVal * static_cast<double>(180.0 / glm::pi<float>()));
        }

        enum Unit
        {
            Radians = 0,
            Degrees = 1
        };

        angle_t(T val, Unit eUnit = Unit::Radians)
        {
            if (eUnit == Degrees)
                mData = deg2Rad(val);
            else
                mData = val;
        }

        angle_t() :
            mData(0)
        {}
        /// value (always in radians)
        T mData;

        operator T() const
        {
            return mData;
        }

        T sin() const
        {
            return glm::sin(mData);
        }

        T tan() const
        {
            return glm::tan(mData);
        }

        T cos() const
        {
            return glm::cos(mData);
        }

        T degrees() const
        {
            return rad2Deg(mData);
        }
    };


    // --- helper types --- //
    typedef angle_t<float> anglef;
    typedef angle_t<double> angled;

    typedef vec<2, float> geo2f;
    typedef vec<3, float> geo3f;
    typedef vec<2, angle_t<double>> geo2d;
    typedef vec<3, angle_t<double>> geo3d;

}