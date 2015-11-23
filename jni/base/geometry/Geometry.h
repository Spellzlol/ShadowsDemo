#pragma once

#include <cstring>

//----------------------------------------------------------------------------------------------------------------
// Vec2
//----------------------------------------------------------------------------------------------------------------

class Vec2
{
public:
	float x = 0.0f;
	float y = 0.0f;

	bool operator== (const Vec2& rhs);
	bool operator!= (const Vec2& rhs);

public:
	Vec2();
	Vec2(float x, float y);
};

typedef Vec2 Size;

//----------------------------------------------------------------------------------------------------------------
// Vec3
//----------------------------------------------------------------------------------------------------------------

class Vec3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	bool operator== (const Vec3& rhs);
	bool operator!= (const Vec3& rhs);

public:
	Vec3();
	Vec3(float x, float y, float z);
};

typedef Vec3 Color3;
typedef Vec3 Size3D;

//----------------------------------------------------------------------------------------------------------------
// Vec4
//----------------------------------------------------------------------------------------------------------------

class Vec4
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	bool operator== (const Vec4& rhs);
	bool operator!= (const Vec4& rhs);

public:
	Vec4();
	Vec4(float x, float y, float z, float w);
};

typedef Vec4 Color4;

//----------------------------------------------------------------------------------------------------------------
// Mat4
//----------------------------------------------------------------------------------------------------------------

class Mat4
{
public:
    Mat4();
    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    static Mat4* createPerspective(float fov, float aspect, float near, float far);
    static Mat4* createOrthographic(float right, float top, float near, float far, float left = 0, float bottom = 0);
    static Mat4* createTranslation(const Vec3& position);

public:
    const float* getData() const;
    float operator[] (size_t pos);

private:
	float m_data[16];
};