#pragma once

namespace glm
{
    template<typename T>
    inline int intersect(const glm::sphere_t<T>& sphere, const glm::ray_t<T>& ray, T& t0, T& t1)
    {

        // set up quadratic Q(t) = a*t^2 + 2*b*t + c
        const glm::vec<3, T> offset = ray.getOrigin() - sphere.getCenter();
        const T a = glm::lensq(ray.getDir());
        const T b = glm::dot(offset, ray.getDir());
        const T c = glm::lensq(offset) - sphere.getRadius() * sphere.getRadius();

        // no intersection if Q(t) has no real roots
        const T discriminant = b * b - a * c;
        if (discriminant < 0.0f)
        {
            return 0;
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
                return 2;
            }
            else if (t1 >= T(0))
            {
                t0 = t1;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            t0 = -b / a;
            if (t0 >= T(0))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }


    template<typename T> bool intersect(const plane_t<T>& p1, const plane_t<T>& p2, vec<3, T>& out_point, vec<3, T>& out_direction)
    {
        // logically the 3rd plane, but we only use the normal component.
        const glm::vec<3, T> lineDir = cross(p1.normal, p2.normal);
        const T det = glm::lensq(lineDir);

        // If the determinant is 0, that means parallel planes, no intersection.
        // note: you may want to check against an epsilon value here.
        if (det == 0.0)
            return false;

        // Find a point on the line by solving the system of equations
        // We use the fact that any point on the intersection line satisfies both plane equations
        T d1 = -p1.d;
        T d2 = -p2.d;

        // Create a system of linear equations to solve for a point on the line
        glm::mat3 A(p1.normal, p2.normal, lineDir);
        glm::vec3 b(-d1, -d2, 0.0f);

        // Solve the system using matrix inverse
        out_point = b * glm::inverse(A);
        out_direction = lineDir;
        return true;
    }

}