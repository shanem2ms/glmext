#pragma once

namespace glm
{

/**
 * Describes a sphere in 3D space by its center point and its radius.
 *
 * @param T     the internal type used for the point and radius
 * @ingroup Types
 */
template<class T>
class sphere_t
{
public:
   typedef T DataType;

public:
   /**
    * Constructs a sphere centered at the origin with a radius of 0.
    */
   sphere_t()
      : mRadius( 0 )
   {}

   /**
    * Constructs a sphere with the given center and radius.
    *
    * @param center     the point at which to center the sphere
    * @param radius     the radius of the sphere
    */
   sphere_t( const vec<3, T>& center, const T& radius )
      : mCenter( center ), mRadius( radius )
   {}

   /**
    * Constructs a duplicate of the given sphere.
    *
    * @param sphere     the sphere to make a copy of
    */
   sphere_t( const sphere_t<T>& sphere )
      : mCenter( sphere.mCenter ), mRadius( sphere.mRadius )
   {}

   /**
    * Gets the center of the sphere.
    *
    * @return  the center point of the sphere
    */
   const vec<3, T>& getCenter() const
   {
      return mCenter;
   }

   /**
    * Gets the radius of the sphere.
    *
    * @return  the radius of the sphere
    */
   const T& getRadius() const
   {
      return mRadius;
   }

   /**
    * Sets the center point of the sphere.
    *
    * @param center     the new point at which to center the sphere
    */
   void setCenter( const vec<3, T>& center )
   {
      mCenter = center;
   }

   /**
    * Sets the radius of the sphere.
    *
    * @param radius     the new radius of the sphere
    */
   void setRadius( const T& radius )
   {
      mRadius = radius;
   }

   /**
   * Tests if the given aabox_t and Sphere intersect with each other. On an edge
   * IS considered intersection by this algorithm.
   *
   * @param box  the box to test
   * @param sph  the sphere to test
   *
   * @return  true if the items intersect; false otherwise
   */
   bool intersectsAABox(const aabox_t<T>& box) const
   {
       T dist_sqr = T(0);

       // Compute the square of the distance from the sphere to the box
       for (int i = 0; i < 3; ++i)
       {
           if (getCenter()[i] < box.getMin()[i])
           {
               T s = getCenter()[i] - box.getMin()[i];
               dist_sqr += s * s;
           }
           else if (getCenter()[i] > box.getMax()[i])
           {
               T s = getCenter()[i] - box.getMax()[i];
               dist_sqr += s * s;
           }
       }

       return dist_sqr <= (getRadius() * getRadius());
   }

public:
   /**
    * The center of the sphere.
    */
   vec<3, T> mCenter;

   /**
    * The radius of the sphere.
    */
   T mRadius;
};

// --- helper types --- //
typedef sphere_t<float>   spheref;
typedef sphere_t<double>  sphered;

};
