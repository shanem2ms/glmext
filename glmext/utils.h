
namespace glm 
{
template <typename T> qua<T> quat_from_axisangle(T angle, const vec<3, T> axis)
{
    T half_angle = angle * (T)0.5;
    T sin_half_angle = glm::sin(half_angle);

    qua<T> result;
    result[0] = glm::cos(half_angle);
    result[1] = sin_half_angle * axis[0];
    result[2] = sin_half_angle * axis[1];
    result[3] = sin_half_angle * axis[2];

    // should automagically be normalized (unit magnitude) now...
    return result;
}
}