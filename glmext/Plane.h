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
      : mOffset( 0 )
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

      glm::cross( mNorm, vec12, vec13 );
      normalize( mNorm );

      mOffset = dot( static_cast< glm::vec<3, T> >(pt1), mNorm );  // Graphics Gems I: Page 390
   }

   /**
    * Creates a plane with the given normal on which pt resides.
    *
    * @param norm    the normal of the plane
    * @param pt      a point that lies on the plane
    */
   plane_t( const glm::vec<3, T>& norm, const glm::vec<3, T>& pt )
      : mNorm( norm )
   {
      mOffset = dot( static_cast< glm::vec<3, T> >(pt), norm );
   }

   /**
    * Creates a plane with the given normal and offset.
    *
    * @param norm          the normal of the plane
    * @param dPlaneConst   the plane offset constant
    */
   plane_t( const glm::vec<3, T>& norm, const T& dPlaneConst )
      : mNorm( norm ), mOffset( dPlaneConst )
   {}

   /**
    * Creates an exact duplicate of the given plane.
    *
    * @param plane   the plane to copy
    */
   plane_t( const plane_t<T>& plane )
      : mNorm( plane.mNorm ), mOffset( plane.mOffset )
   {}

   /**
    * Gets the normal for this plane.
    *
    * @return  this plane's normal vector
    */
   const glm::vec<3, T>& getNormal() const
   {
      return mNorm;
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
      mNorm = norm;
   }

   /**
    * Gets the offset of this plane from the origin such that the offset is the
    * negative distance from the origin.
    *
    * @return  this plane's offset
    */
   const T& getOffset() const
   {
      return mOffset;
   }

   /**
    * Sets the offset of this plane from the origin.
    *
    * @param offset     the new offset
    */
   void setOffset( const T& offset )
   {
      mOffset = offset;
   }

   
   T distanceTo(const vec<3, T>& pt) const
   {
       return (dot(mNorm, pt) - mOffset);
   }

public:
   // dot(Pt,mNorm) = mOffset
   /**
    * The normal for this vector. For any point on the plane,
    * dot( pt, mNorm) = mOffset.
    */
   glm::vec<3, T> mNorm;

   /**
    * This plane's offset from the origin such that for any point pt,
    * dot( pt, mNorm ) = mOffset. Note that mOffset = -D (neg dist from the
    * origin).
    */
   T mOffset;
};

typedef plane_t<float> planef;
typedef plane_t<double> planed;

/*
#include <geomdist.h>


// Intersects the plane with a given segment.
// Returns TRUE if there is a hit (within the seg).
// Also returns the distance "down" the segment of the hit in t.
//
// PRE: seg.dir must be normalized
//
int sgPlane::isect(sgSeg& seg, float* t)
{
	// Graphic Gems I: Page 391
	float denom = normal.dot(seg.dir);
	if (SG_IS_ZERO(denom))		// No Hit
	{
		//: So now, it is just dist to plane tested against length
		sgVec3	hit_pt;
		float		hit_dist;		// Dist to hit
		
		hit_dist = findNearestPt(seg.pos, hit_pt);
		*t = hit_dist;			// Since dir is normalized

		if(seg.tValueOnSeg(*t))
			return 1;
		else
			return 0;
	}
	else
	{
		float numer = offset + normal.dot(seg.pos);
		(*t) = -1.0f * (numer/denom);
		
		if(seg.tValueOnSeg(*t))
			return 1;
		else
			return 0;
	}
}

///
 // Intersects the plane with the line defined
 // by the given segment
 //
 // seg - seg that represents the line to isect
 // t   - the t value of the isect
 //
int sgPlane::isectLine(const sgSeg& seg, float* t)
{
	// GGI: Pg 299
	// Lu = seg.pos;
	// Lv = seg.dir;
	// Jn = normal;
	// Jd = offset;
	
	float denom = normal.dot(seg.dir);
	if(denom == 0.0f)
      return 0;
	else
	{
      *t = - (offset+ normal.dot(seg.pos))/(denom);
      return 1;
	}
}
*/
typedef plane_t<float> planef;
typedef plane_t<double> planed;
}
