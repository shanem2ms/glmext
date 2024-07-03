#pragma once

namespace glm {

/// Standalone axis aligned bounding box implemented built on top of GLM.
template<class T>
class aabox2_t
{
public:
  /// Builds a null aabox2_t.
  aabox2_t()
  {
      setNull();
  }

  aabox2_t(const glm::vec2& center, T radius)
  {
      setNull();
      extend(center, radius);
  }

  aabox2_t(const glm::vec2& p1, const glm::vec2& p2)
  {
      setNull();
      extend(p1);
      extend(p2);
  }

  aabox2_t(const aabox2_t& aabb)
  {
      setNull();
      extend(aabb);
  }

  ~aabox2_t()
  {
  }

  /// Set the aabox2_t as NULL (not set).
  void setNull()      {mMin = glm::vec<2, T>(std::numeric_limits<T>::max()); mMax = glm::vec<2, T>(std::numeric_limits<T>::lowest());}

  /// Returns true if aabox2_t is NULL (not set).
  bool isNull() const {return mMin.x > mMax.x || mMin.y > mMax.y;}

  /// Extend the bounding box on all sides by \p val.
  void extend(T val)
  {
      if (!isNull())
      {
          mMin -= glm::vec<2, T>(val);
          mMax += glm::vec<2, T>(val);
      }
  }

  void extend(const glm::vec<2, T>& p)
  {
      if (!isNull())
      {
          mMin = glm::min(p, mMin);
          mMax = glm::max(p, mMax);
      }
      else
      {
          mMin = p;
          mMax = p;
      }
  }

  /// Expand the aabox2_t to include a sphere centered at \p center and of radius \p
  /// radius.
  /// \param[in]  center Center of sphere.
  /// \param[in]  radius Radius of sphere.
  void extend(const glm::vec<2, T>& center, T radius);

  /// Expand the aabox2_t to encompass the given \p aabb.
  void extend(const aabox2_t& aabb);

  /// Expand the aabox2_t to include a disk centered at \p center, with normal \p
  /// normal, and radius \p radius.
  /// \xxx Untested -- This function is not represented in our unit tests.
  void extendDisk(const glm::vec<2, T>& center, const glm::vec<2, T>& normal,
                  T radius);

  /// Translates aabox2_t by vector \p v.
  void translate(const glm::vec<2, T>& v);

  /// Scale the aabox2_t by \p scale, centered around \p origin.
  /// \param[in]  scale  3D vector specifying scale along each axis.
  /// \param[in]  origin Origin of scaling operation. Most useful origin would
  ///                    be the center of the aabox2_t.
  void scale(const glm::vec<2, T>& scale, const glm::vec<2, T>& origin);

  /// Retrieves the center of the aabox2_t.
  glm::vec<2, T> getCenter() const
  {
      return (mMin + mMax) * (T)(0.5);
  }

  /// Retrieves the diagonal vector (computed as mMax - mMin).
  /// If the aabox2_t is NULL, then a vector of all zeros is returned.
  glm::vec<2, T> getDiagonal() const;


  glm::vec<2, T> extents() const
  { return mMax - mMin; }

  /// Retrieves the longest edge.
  /// If the aabox2_t is NULL, then 0 is returned.
  T getLongestEdge() const;

  /// Retrieves the shortest edge.
  /// If the aabox2_t is NULL, then 0 is returned.
  T getShortestEdge() const;

  /// Retrieves the aabox2_t's minimum point.
  glm::vec<2, T> getMin() const {return mMin;}

  /// Retrieves the aabox2_t's maximum point.
  glm::vec<2, T> getMax() const {return mMax;}
  glm::vec<2, T> center() const { return (mMax + mMin) * (T)0.5; }

  /// Returns true if AABBs share a face overlap.
  /// \xxx Untested -- This function is not represented in our unit tests.
  bool overlaps(const aabox2_t& bb) const;

  /// Type returned from call to intersect.
  enum INTERSECTION_TYPE { INSIDE, INTERSECT, OUTSIDE };
  /// Returns one of the intersection types. If either of the aabbs are invalid,
  /// then OUTSIDE is returned.
  INTERSECTION_TYPE intersect(const aabox2_t& bb) const;

  bool contains(const glm::vec<2, T>& pt) const;

  template<int IDX> glm::vec<2, T> corner()
  {
      return glm::vec<2, T>((IDX & 2) ? getMin()[0] : getMax()[0],
          (IDX & 1) ? getMin()[1] : getMax()[1]);
  }

public:

  glm::vec<2, T> mMin;   ///< Minimum point.
  glm::vec<2, T> mMax;   ///< Maximum point.
};

template< class T >
inline aabox2_t<T>& operator += (aabox2_t<T>& b1, const vec<2, T>& p)
{
    for (int i = 0; i < 2; ++i)
    {
        if (p[i] > b1.mMax[i])
            b1.mMax[i] = p[i];
        if (p[i] < b1.mMin[i])
            b1.mMin[i] = p[i];
    }
    return b1;
}


template< class T >
inline aabox2_t<T>& operator += (aabox2_t<T>& b1, const aabox2_t<T>& b2)
{
    b1 += (const vec<2, T>&)b2.mMin;
    b1 += (const vec<2, T>&)b2.mMax;
    return b1;
}

typedef aabox2_t<double> aabox2d;
typedef aabox2_t<float> aabox2f;
} // namespace CPM_GLM_AABB_NS 

