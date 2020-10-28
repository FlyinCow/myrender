#ifndef _VECTOR_H_
#define _VECTOR_H_

template <class T>
struct Vec2
{
    union
    {
        struct
        {
            T x;
            T y;
        };
        T raw[2];
    };

    inline Vec2(T x, T y) : x(x), y(y) {}
    inline Vec2() : Vec2(T(), T()) {}
    inline Vec2<T> &operator+=(const Vec2<T> &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    inline Vec2<T> operator+(const Vec2<T> &rhs)
    {
        return {x + rhs.x, y + rhs.y};
    }
    inline Vec2<T> operator-(const Vec2<T> &rhs)
    {
        return {x - rhs.x, y - rhs.y};
    }
    inline Vec2<T> &operator-=(const Vec2<T> &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    inline const Vec2<T> &operator*=(float k)
    {
        x *= k;
        y *= k;
        return *this;
    }

    inline T operator*(const Vec2<T> &v)
    {
        return x * v.x + y * v.y;
    }
};

template <class T>
inline Vec2<T> operator*(float k, Vec2<T> v)
{
    return Vec2<T>{v.x * k, v.y * k};
}

template <class T>
inline Vec2<T> operator*(Vec2<T> v, T k)
{
    return Vec2<T>{v.x * k, v.y * k};
}

/****************************************************************************/

template <class T>
struct Vec3
{
    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        T raw[3];
    };

    inline Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    inline Vec3() : Vec3(T(), T(), T()) {}
    inline Vec3<T> &operator+=(const Vec3<T> &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    inline Vec3<T> operator+(const Vec3<T> &rhs)
    {
        return {x + rhs.x, y + rhs.y};
    }
    inline Vec3<T> operator-(const Vec3<T> &rhs)
    {
        return {x - rhs.x, y - rhs.y};
    }
    inline Vec3<T> &operator-=(const Vec3<T> &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    inline const Vec3<T> &operator*=(float k)
    {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    inline T operator*(const Vec3<T> &v)
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec2<T> xy() const { return {x, y}; };
    Vec2<T> yz() const { return {y, z}; };
    Vec2<T> xz() const { return {x, z}; };
};

template <class T>
inline Vec3<T> operator*(float k, Vec3<T> v)
{
    return Vec3<T>{v.x * k, v.y * k, v.z * k};
}

template <class T>
inline Vec3<T> operator*(Vec3<T> v, T k)
{
    return Vec3<T>{v.x * k, v.y * k, v.z * k};
}

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;

#endif // _VECTOR_H_