#pragma once

namespace glm
{

/**
 *  plane_t: Defines a geometrical plane.
 *
 *  All points on the plane satify the equation dot(Pt,Normal) = offset
 *  normal is assumed to be normalized
 *
 *  NOTE: Some plane implementation store D instead of offset.  Thus
 *       those implementation have opposite sign from what we have
 *
 * pg. 309 Computer Graphics 2nd Edition Hearn Baker
 *<pre>
 *  N dot P = -D
 *   |
 *   |-d-|
 * __|___|-->N
 *   |   |
 *</pre>
 * @ingroup Types
 */
template< class T>
class plane_t
{
public:
   /**
    * Creates an uninitialized plane_t. In other words, the normal is (0,0,0) and
    * the offset is 0.
    */
   plane_t()
      : d( 0 )
   {}

   /**
    * Creates a plane that the given points lie on.
    *
    * @param pt1     a point on the plane
    * @param pt2     a point on the plane
    * @param pt3     a point on the plane
    */
   plane_t( const glm::vec<3, T>& pt1, const glm::vec<3, T>& pt2,
          const glm::vec<3, T>& pt3)
   {
      glm::vec<3, T> vec12( pt2-pt1 );
      glm::vec<3, T> vec13( pt3-pt1 );

      glm::cross( normal, vec12, vec13 );
      normalize( normal );

      d = dot( static_cast< glm::vec<3, T> >(pt1), normal );  // Graphics Gems I: Page 390
   }

   /**
    * Creates a plane with the given normal on which pt resides.
    *
    * @param norm    the normal of the plane
    * @param pt      a point that lies on the plane
    */
   plane_t( const glm::vec<3, T>& norm, const glm::vec<3, T>& pt )
      : normal( norm )
   {
      d = dot( static_cast< glm::vec<3, T> >(pt), norm );
   }

   /**
    * Creates a plane with the given normal and offset.
    *
    * @param norm          the normal of the plane
    * @param dPlaneConst   the plane offset constant
    */
   plane_t( const glm::vec<3, T>& norm, const T& dPlaneConst )
      : normal( norm ), d( dPlaneConst )
   {}

   /**
    * Creates an exact duplicate of the given plane.
    *
    * @param plane   the plane to copy
    */
   plane_t( const plane_t<T>& plane )
      : normal( plane.normal ), d( plane.d )
   {}

   /**
    * Gets the normal for this plane.
    *
    * @return  this plane's normal vector
    */
   const glm::vec<3, T>& getNormal() const
   {
      return normal;
   }

   /**
    * Sets the normal for this plane to the given vector.
    *
    * @param norm    the new normalized vector
    *
    * @pre |norm| = 1
    */
   void setNormal( const glm::vec<3, T>& norm )
   {
      normal = norm;
   }

   /**
    * Gets the offset of this plane from the origin such that the offset is the
    * negative distance from the origin.
    *
    * @return  this plane's offset
    */
   const T& getOffset() const
   {
      return d;
   }

   /**
    * Sets the offset of this plane from the origin.
    *
    * @param offset     the new offset
    */
   void setOffset( const T& offset )
   {
      d = offset;
   }

   
   T distanceTo(const vec<3, T>& pt) const
   {
       return (dot(normal, pt) - d);
   }

public:
   // dot(Pt,normal) = d
   /**
    * The normal for this vector. For any point on the plane,
    * dot( pt, normal) = d.
    */
   glm::vec<3, T> normal;

   /**
    * This plane's offset from the origin such that for any point pt,
    * dot( pt, normal ) = d. Note that d = -D (neg dist from the
    * origin).
    */
   T d;
};

template<typename T> vec<3, T> generate_u(const plane_t<T>& p)
{
    if (std::abs(p.normal.x) > std::abs(p.normal.z)) {
        return normalize(vec<3, T>(-p.normal.y, p.normal.x, 0.0f));
    }
    else {
        return normalize(vec<3, T>(0.0f, -p.normal.z, p.normal.y));
    }
}

template<typename T> void generate_uv(const plane_t<T>& p, vec<3, T> &out_u, vec<3, T> &out_v)
{
    out_u = generate_u(p);
    out_v = cross(out_u, p.normal);
}


template<typename T> vec<2, T> project(const vec<3, T>& u, const vec<3, T>& v, const vec<3, T>& invec)
{
    return vec<2, T>(dot(u, invec), dot(v, invec));
}

template<typename T> vec<3, T> unproject(
    const plane_t<T> &p,
    const vec<3, T>& u, const vec<3, T>& v, const vec<2, T>& invec)
{
    return invec.x * u + invec.y * v + p.normal * p.d;
}

typedef plane_t<float> planef;
typedef plane_t<double> planed;
}
