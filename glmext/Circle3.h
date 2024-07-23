#pragma once

namespace glm
{
    template <typename T> class circle3_t
    {
    public:

        glm::vec<3, T> center;
        glm::vec<3, T> normal;
        T radius;

        circle3_t(const glm::vec<3, T>& c,
            const glm::vec<3, T>& n,
            T r) : center(c),
            normal(n),
            radius(r) {}
        circle3_t() {}

        T AngleFromPt(const glm::vec<3, T>& p) const
        {
            glm::vec<3, T> v = glm::normalize(p - center);
            glm::plane_t<T> circle_plane(normal, center);
            glm::vec<3, T> udir, vdir;
            glm::generate_uv(circle_plane, udir, vdir);
            T angle = glm::atan(glm::dot(v, vdir), glm::dot(v, udir));
            if (angle < 0) angle += 2 * glm::pi<T>();
            return angle;
        }

        glm::vec<3, T> PtFromAngle(T angle) const
        {
            glm::plane_t<T> circle_plane(normal, center);
            glm::vec<3, T> udir, vdir;
            glm::generate_uv(circle_plane, udir, vdir);
            return center + radius * (udir * std::cos(angle) + vdir * std::sin(angle));
        }

        std::vector<glm::vec<3, T>> discretize(int numSegments, T a0 = 0, T a1 = 0) const
        {
            if (a1 < a0)
                a1 += glm::two_pi<T>();
            std::vector<glm::vec<3, T>> points;
            T delta = a1 - a0;
            if (delta <= 0)
                delta += glm::two_pi<T>();
            T angleStep = delta / (numSegments - 1);
            glm::plane_t<T> circle_plane(normal, center);
            glm::vec<3, T> udir, vdir;
            glm::generate_uv(circle_plane, udir, vdir);

            for (int i = 0; i < numSegments; ++i) {
                T angle = a0 + i * angleStep;
                glm::vec3 point = center + radius * (udir * std::cos(angle) + vdir * std::sin(angle));
                points.push_back(point);
            }

            return points;
        }
    };


    typedef circle3_t<float> circle3f;

}