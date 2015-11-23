#include "Geometry.h"
#include "utils.h"
#include <cmath>
#include <new>

//----------------------------------------------------------------------------------------------------------------
// Vec2
//----------------------------------------------------------------------------------------------------------------

Vec2::Vec2()
{

}

Vec2::Vec2(float x, float y): x(x), y(y)
{

}

bool Vec2::operator== (const Vec2& rhs)
{
    return rhs.x == this->x and rhs.y == this->y;
}

bool Vec2::operator!= (const Vec2& rhs)
{
    return rhs.x != this->x or rhs.y != this->y;
}

//----------------------------------------------------------------------------------------------------------------
// Vec3
//----------------------------------------------------------------------------------------------------------------

Vec3::Vec3()
{

}

Vec3::Vec3(float x, float y, float z): x(x), y(y), z(z)
{

}

bool Vec3::operator== (const Vec3& rhs)
{
    return rhs.x == this->x and rhs.y == this->y and rhs.z == this->z;
}

bool Vec3::operator!= (const Vec3& rhs)
{
    return rhs.x != this->x or rhs.y != this->y or rhs.z != this->z;
}

//----------------------------------------------------------------------------------------------------------------
// Vec4
//----------------------------------------------------------------------------------------------------------------

Vec4::Vec4()
{

}

Vec4::Vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
{

}

bool Vec4::operator== (const Vec4& rhs)
{
    return rhs.x == this->x and rhs.y == this->y and rhs.z == this->z and rhs.w == this->w;
}

bool Vec4::operator!= (const Vec4& rhs)
{
    return rhs.x != this->x or rhs.y != this->y or rhs.z != this->z or rhs.w != this->w;
}

//----------------------------------------------------------------------------------------------------------------
// Mat4
//----------------------------------------------------------------------------------------------------------------

Mat4::Mat4()
{
	m_data[0]  = 0;
    m_data[1]  = 0;
    m_data[2]  = 0;
    m_data[3]  = 0;
    m_data[4]  = 0;
    m_data[5]  = 0;
    m_data[6]  = 0;
    m_data[7]  = 0;
    m_data[8]  = 0;
    m_data[9]  = 0;
    m_data[10] = 0;
    m_data[11] = 0;
    m_data[12] = 0;
    m_data[13] = 0;
    m_data[14] = 0;
    m_data[15] = 0;
}

Mat4::Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
	m_data[0]  = m11;
    m_data[1]  = m21;
    m_data[2]  = m31;
    m_data[3]  = m41;
    m_data[4]  = m12;
    m_data[5]  = m22;
    m_data[6]  = m32;
    m_data[7]  = m42;
    m_data[8]  = m13;
    m_data[9]  = m23;
    m_data[10] = m33;
    m_data[11] = m43;
    m_data[12] = m14;
    m_data[13] = m24;
    m_data[14] = m34;
    m_data[15] = m44;
}

Mat4* Mat4::createTranslation(const Vec3& position)
{
    Mat4* result = new (std::nothrow) Mat4();

    if (not result)
    {
        return nullptr;
    }

    result->m_data[0]  = 1.0f;
    result->m_data[3]  = position.x;
    result->m_data[5]  = 1.0f;
    result->m_data[7]  = position.y;
    result->m_data[10] = 1.0f;
    result->m_data[11] = position.z;
    result->m_data[15] = 1.0f;

    return result;
}

Mat4* Mat4::createPerspective(float fov, float aspect, float near, float far)
{
    Mat4* result = new (std::nothrow) Mat4();

    if (not result)
    {
        return nullptr;
    }

    if ( fov <= 0 || aspect == 0 )
    {
        Assert(fov > 0 && aspect != 0,  "Wrong params for projection matrix!");
        return result;
    }

    float frustumDepth = far - near;
    float oneOverDepth = 1 / frustumDepth;

    result->m_data[5] = 1 / tan(0.5f * fov);
    result->m_data[0] = result->m_data[5] / aspect;
    result->m_data[10] = far * oneOverDepth;
    result->m_data[14] = (-far * near) * oneOverDepth;
    result->m_data[11] = 1;

    return result;
}

Mat4* Mat4::createOrthographic(float right, float top, float near, float far, float left /* = 0 */, float bottom /* = 0 */)
{
    Mat4* result = new (std::nothrow) Mat4();

    if (not result)
    {
        return nullptr;
    }

    result->m_data[0] = 2 / (right - left);
    result->m_data[5] = 2 / (top - bottom);
    result->m_data[10] = 2 / (near - far);

    result->m_data[12] = (left + right) / (left - right);
    result->m_data[13] = (top + bottom) / (bottom - top);
    result->m_data[14] = (near + far) / (near - far);
    result->m_data[15] = 1;

    return result;
}

const float* Mat4::getData() const
{
    return m_data;
}

float Mat4::operator[] (size_t pos)
{
    if (pos >= 16)
    {
        Assert(false, "Index out of range! Got: %d, Max: 15", pos);
        return 0.0f;
    }

    return m_data[pos];
}