#pragma once

namespace glm {

/**
 * Describes a ray. This is represented by a point origin O and a
 * normalized vector direction. Any point on the ray can be described as
 *
 * P(s) = O + Vs
 *
 * where 0 <= s <= 1
 *
 * @param T     the internal type used for the point and vector
 */
template< class T >
class ray_t
{
public:
   /**
    * Constructs a ray at the origin with a zero vector.
    */
    ray_t()
   {}

   /**
    * Constructs a ray with the given origin and vector.
    *
    * @param origin     the point at which the ray starts
    * @param dir        the vector describing the direction and length of the
    *                   ray starting at origin
    */
    ray_t( const vec<3, T>& origin, const vec<3, T>& dir )
      : mOrigin( origin ), mDir( dir )
   {}

   

   /**
    * Constructs an exact duplicate of the given ray.
    *
    * @param lineseg    the ray to copy
    */
    ray_t( const ray_t& lineseg )
   {
      mOrigin = lineseg.mOrigin;
      mDir = lineseg.mDir;
   }

   /**
    * Gets the origin of the ray.
    *
    * @return  the point at the beginning of the line
    */
   const vec<3, T>& getOrigin() const
   {
      return mOrigin;
   }

   /**
    * Sets the origin point for this ray.
    *
    * @param origin     the point at which the ray starts
    */
   void setOrigin( const vec<3, T>& origin )
   {
      mOrigin = origin;
   }

   /**
    * Gets the vector describing the direction and length of the ray.
    *
    * @return  the ray's vector
    */
   const vec<3, T>& getDir() const
   {
      return mDir;
   }

   /**
    * Sets the vector describing the direction and length of the ray.
    *
    * @param dir     the ray's vector
    */
   void setDir( const vec<3, T>& dir )
   {
      mDir = dir;
   }

   /**
    * intersect ray/sphere-shell (not volume).
    * only register hits with the surface of the sphere.
    * note: after calling this, you can find the intersection point with: ray.getOrigin() + ray.getDir() * t
    *
    * @param ray     the ray to test
    * @param sphere  the sphere to test
    * @return returns intersection point in t, and the number of hits
    * @return numhits, t0, t1 are undefined if return value is false
    */
   inline bool intersect(const sphere_t<T>& sphere, int& numhits, T& t0, T& t1) const
   {
       numhits = -1;

       // set up quadratic Q(t) = a*t^2 + 2*b*t + c
       const vec<3, T> offset = getOrigin() - sphere.getCenter();
       const T a = lensq(getDir());
       const T b = dot(offset, getDir());
       const T c = lensq(offset) - sphere.getRadius() * sphere.getRadius();

       // no intersection if Q(t) has no real roots
       const T discriminant = b * b - a * c;
       if (discriminant < 0.0f)
       {
           numhits = 0;
           return false;
       }
       else if (discriminant > 0.0f)
       {
           T root = glm::sqrt(discriminant);
           T invA = T(1) / a;
           t0 = (-b - root) * invA;
           t1 = (-b + root) * invA;

           // assert: t0 < t1 since A > 0

           if (t0 >= T(0))
           {
               numhits = 2;
               return true;
           }
           else if (t1 >= T(0))
           {
               numhits = 1;
               t0 = t1;
               return true;
           }
           else
           {
               numhits = 0;
               return false;
           }
       }
       else
       {
           t0 = -b / a;
           if (t0 >= T(0))
           {
               numhits = 1;
               return true;
           }
           else
           {
               numhits = 0;
               return false;
           }
       }
   }

   /**
       * Given an axis-aligned bounding box and a ray (or subclass thereof),
       * returns whether the ray intersects the box, and if so, \p tIn and
       * \p tOut are set to the parametric terms on the ray where the segment
       * enters and exits the box respectively.
       *
       * The implementation of this function comes from the book
       * <i>Geometric Tools for Computer Graphics</i>, pages 626-630.
       *
       * @note Internal function for performing an intersection test between an
       *       axis-aligned bounding box and a ray. User code should not call this
       *       function directly. It is used to capture the common code between
       *       the gmtl::Ray<T> and gmtl::LineSeg<T> overloads of
       *       gmtl::intersect() when intersecting with a gmtl::aabox_t<T>.
       */
   bool intersectAABoxRay(const aabox_t<T>& box,
       T& tIn, T& tOut) const
   {
       tIn = -(std::numeric_limits<T>::max)();
       tOut = (std::numeric_limits<T>::max)();
       T t0, t1;
       const T epsilon((T)0.0000001);

       // YZ plane.
       if (glm::abs(mDir[0]) < epsilon)
       {
           // Ray parallel to plane.
           if (mOrigin[0] < box.mMin[0] || mOrigin[0] > box.mMax[0])
           {
               return false;
           }
       }

       // XZ plane.
       if (glm::abs(mDir[1]) < epsilon)
       {
           // Ray parallel to plane.
           if (mOrigin[1] < box.mMin[1] || mOrigin[1] > box.mMax[1])
           {
               return false;
           }
       }

       // XY plane.
       if (glm::abs(mDir[2]) < epsilon)
       {
           // Ray parallel to plane.
           if (mOrigin[2] < box.mMin[2] || mOrigin[2] > box.mMax[2])
           {
               return false;
           }
       }

       // YZ plane.
       t0 = (box.mMin[0] - mOrigin[0]) / mDir[0];
       t1 = (box.mMax[0] - mOrigin[0]) / mDir[0];

       if (t0 > t1)
       {
           std::swap(t0, t1);
       }

       if (t0 > tIn)
       {
           tIn = t0;
       }
       if (t1 < tOut)
       {
           tOut = t1;
       }

       if (tIn > tOut || tOut < T(0))
       {
           return false;
       }

       // XZ plane.
       t0 = (box.mMin[1] - mOrigin[1]) / mDir[1];
       t1 = (box.mMax[1] - mOrigin[1]) / mDir[1];

       if (t0 > t1)
       {
           std::swap(t0, t1);
       }

       if (t0 > tIn)
       {
           tIn = t0;
       }
       if (t1 < tOut)
       {
           tOut = t1;
       }

       if (tIn > tOut || tOut < T(0))
       {
           return false;
       }

       // XY plane.
       t0 = (box.mMin[2] - mOrigin[2]) / mDir[2];
       t1 = (box.mMax[2] - mOrigin[2]) / mDir[2];

       if (t0 > t1)
       {
           std::swap(t0, t1);
       }

       if (t0 > tIn)
       {
           tIn = t0;
       }
       if (t1 < tOut)
       {
           tOut = t1;
       }

       if (tIn > tOut || tOut < T(0))
       {
           return false;
       }

       return true;
   }

   bool intersect(const aabox_t<T>& box, 
       unsigned int& numHits, T& tIn, T& tOut) const
   {
       numHits = 0;

       bool result = intersectAABoxRay(box, tIn, tOut);

       if (result)
       {
           // Ray is inside the box.
           if (tIn < T(0))
           {
               tIn = tOut;
               numHits = 1;
           }
           else
           {
               numHits = 2;
           }
       }

       return result;
   }
public:
   /**
    * The origin of the ray.
    */
   vec<3, T> mOrigin;

   /**
    * The vector along which the ray lies.
    */
   vec<3, T> mDir;
};


// --- helper types --- //
typedef ray_t<float>  rayf;
typedef ray_t<double> rayd;
}
