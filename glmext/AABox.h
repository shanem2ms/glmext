namespace glm
{
    /**
     * Describes an axially aligned box in 3D space. This is usually used for
     * graphics applications. It is defined by its minimum and maximum points.
     *
     * @param T     the internal type used for the points
     *
     * @ingroup Types
     */
    template<class T> class aabox_t
    {
        // This is a hack to work around a bug with GCC 3.3 on Mac OS X
        // where boost::is_polymorphic returns a false positive.  The details
        // can be found in the Boost.Python FAQ:
        //    http://www.boost.org/libs/python/doc/v2/faq.html#macosx
#if defined(__MACH__) && defined(__APPLE_CC__) && defined(__GNUC__) && \
    __GNUC__ == 3 && __GNUC_MINOR__ == 3
        bool dummy_;
#endif
    public:
        typedef T DataType;

    public:
        /**
         * Creates a new empty box.
         */
        aabox_t()
            : mMin(
                std::numeric_limits<T>::max(),
                std::numeric_limits<T>::max(),
                std::numeric_limits<T>::max()),
            mMax(
                std::numeric_limits<T>::lowest(),
                std::numeric_limits<T>::lowest(),
                std::numeric_limits<T>::lowest()),
            mEmpty(true)
        {}

        /**
         * Creates a new box with the given min and max points.
         *
         * @param min     the minimum point on the box
         * @param max     the maximum point on the box
         *
         * @pre  all elements of min are less than max
         * @pre  bot min and max are not zero
         */
        aabox_t(const glm::vec<3, T>& min, const glm::vec<3, T>& max)
            : mMin(min), mMax(max), mEmpty(false)
        {}

        /**
         * Construcst a duplicate of the given box.
         *
         * @param box     the box the make a copy of
         */
        aabox_t(const aabox_t<T>& box)
            : mMin(box.mMin), mMax(box.mMax), mEmpty(box.mEmpty)
        {}

        /**
         * Gets the minimum point of the box.
         *
         * @return  the min point
         */
        const glm::vec<3, T>& getMin() const
        {
            return mMin;
        }

        /**
         * Gets the maximum point of the box.
         *
         * @return  the max point
         */
        const glm::vec<3, T>& getMax() const
        {
            return mMax;
        }

        /**
         * Tests if this box occupies no space.
         *
         * @return  true if the box is empty, false otherwise
         */
        bool isEmpty() const
        {
            return mEmpty;
        }

        /**
         * Sets the minimum point of the box.
         *
         * @param min     the min point
         */
        void setMin(const glm::vec<3, T>& min)
        {
            mMin = min;
        }

        /**
         * Sets the maximum point of the box.
         *
         * @param max     the max point
         */
        void setMax(const glm::vec<3, T>& max)
        {
            mMax = max;
        }

        /**
         * Sets the empty flag on this box.
         *
         * @param empty   true to make the box empty, false otherwise
         */
        void setEmpty(bool empty)
        {
            mEmpty = empty;
        }

        bool intersects(const aabox_t<T>& box2) const
        {
            // Look for a separating axis on each box for each axis
            if (getMin()[0] > box2.getMax()[0])  return false;
            if (getMin()[1] > box2.getMax()[1])  return false;
            if (getMin()[2] > box2.getMax()[2])  return false;

            if (box2.getMin()[0] > getMax()[0])  return false;
            if (box2.getMin()[1] > getMax()[1])  return false;
            if (box2.getMin()[2] > getMax()[2])  return false;

            // No separating axis ... they must intersect
            return true;
        }

        bool intersects(const vec<3, T>& point) const
        {
            if (getMin()[0] > point[0])  return false;
            if (getMin()[1] > point[1])  return false;
            if (getMin()[2] > point[2])  return false;

            if (point[0] > getMax()[0])  return false;
            if (point[1] > getMax()[1])  return false;
            if (point[2] > getMax()[2])  return false;

            // they must intersect
            return true;
        }
    public:
        /**
         * The minimum point of the box.
         */
        glm::vec<3, T> mMin;

        /**
         * The maximum point on the box.
         */
        glm::vec<3, T> mMax;

        /**
         * Flag for empty box. True if the box is empty.
         */
        bool mEmpty;
    };

/** @ingroup Compare aabox_t 
 * @name aabox_t Comparitors
 * @{
 */

/**
 * Compare two AABoxes to see if they are EXACTLY the same. In other words,
 * this comparison is done with zero tolerance.
 *
 * @param b1      the first box to compare
 * @param b2      the second box to compare
 *
 * @return  true if they are equal, false otherwise
 */
template< class T >
inline bool operator==( const aabox_t<T>& b1, const aabox_t<T>& b2 )
{
   return ( (b1.isEmpty() == b2.isEmpty()) &&
            (b1.getMin() == b2.getMin()) &&
            (b1.getMax() == b2.getMax()) );
}

/**
 * Compare two AABoxes to see if they are not EXACTLY the same. In other words,
 * this comparison is done with zero tolerance.
 *
 * @param b1      the first box to compare
 * @param b2      the second box to compare
 *
 * @return  true if they are not equal, false otherwise
 */
template< class T >
inline bool operator!=( const aabox_t<T>& b1, const aabox_t<T>& b2 )
{
   return (! (b1 == b2));
}


template< class T >
inline aabox_t<T> &operator += (aabox_t<T>& b1, const glm::vec<3, T>& p)
{
    for (int i = 0; i < 3; ++i)
    {
        if (p[i] > b1.mMax[i])
            b1.mMax[i] = p[i];
        if (p[i] < b1.mMin[i])
            b1.mMin[i] = p[i];
    }
    b1.mEmpty = false;
    return b1;
}



/**
 * Compare two AABoxes to see if they are the same within the given tolerance.
 *
 * @param b1      the first box to compare
 * @param b2      the second box to compare
 * @param eps     the tolerance value to use
 *
 * @pre eps must be >= 0
 *
 * @return  true if their points are within the given tolerance of each other, false otherwise
 */
template< class T >
inline bool isEqual( const aabox_t<T>& b1, const aabox_t<T>& b2, const T& eps )
{
gmtlASSERT( eps >= 0 );
return (b1.isEmpty() == b2.isEmpty()) &&
        isEqual( b1.getMin(), b2.getMin(), eps ) &&
        isEqual( b1.getMax(), b2.getMax(), eps );
}
/** @} */

template< class T >
inline void getCorners(const aabox_t<T>& a, glm::vec<3, T> corners[8])
{
    corners[0] = glm::vec<3, T>(a.mMin[0], a.mMin[1], a.mMin[2]);
    corners[1] = glm::vec<3, T>(a.mMin[0], a.mMin[1], a.mMax[2]);
    corners[2] = glm::vec<3, T>(a.mMin[0], a.mMax[1], a.mMin[2]);
    corners[3] = glm::vec<3, T>(a.mMin[0], a.mMax[1], a.mMax[2]);
    corners[4] = glm::vec<3, T>(a.mMax[0], a.mMin[1], a.mMin[2]);
    corners[5] = glm::vec<3, T>(a.mMax[0], a.mMin[1], a.mMax[2]);
    corners[6] = glm::vec<3, T>(a.mMax[0], a.mMax[1], a.mMin[2]);
    corners[7] = glm::vec<3, T>(a.mMax[0], a.mMax[1], a.mMax[2]);
}

template< class T >
inline glm::vec<3, T> extents(const aabox_t<T>& b1)
{
    return b1.getMax() - b1.getMin();
}

template< class T >
inline glm::vec<3, T> middle(const aabox_t<T>& b1)
{
    return (b1.getMax() + b1.getMin()) * static_cast<T>(0.5);
}

// --- helper types --- //
typedef aabox_t<float>    aaboxf;
typedef aabox_t<double>   aaboxd;    
}
