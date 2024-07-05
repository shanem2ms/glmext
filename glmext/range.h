#pragma once

namespace glm
{
    template <typename T> class range_t
    {
    public:
        T min;
        T max;

        range_t() :
            max(-std::numeric_limits<T>::max()),
            min(std::numeric_limits<T>::max())
        {}

        range_t(T a, T b) :
            min(a),
            max(b) {}


        bool empty() const {
            return max < min;
        }


        range_t intersect(range_t other)
        {
            return
                range_t(std::max(min, other.min), std::min(max, other.max));
        }

        range_t& operator +=(T val)
        {
            min = std::min(min, val);
            max = std::max(max, val);
            return *this;
        }

        range_t& offset(T val)
        {
            min += val;
            max += val;
            return *this;
        }

        T normalize(T input) const
        {
            return (input - min) / (max - min);
        }
    };

    typedef range_t<float> rangef;
}