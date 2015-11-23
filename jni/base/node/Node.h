#pragma once

#include "Geometry.h"
#include <memory>
#include "GLProgram.h"

//----------------------------------------------------------------------------------------------------------------
// Node
//----------------------------------------------------------------------------------------------------------------

class Node
{
public:
	Node() = default;
	virtual ~Node() = default;

	const Vec3& getPosition() const;
	void setPosition(const Vec3& newPos);
	void setPosition(float x, float y, float z);

	const Vec2& getScale() const;
	void setScale(const Vec2& scale);
	void setScale(float x, float y);

	void setColor(const Color4& color);
	const Color4& getColor() const;

	void setSize(const Vec2& size);
	const Size& getSize() const;

	void setRotation(float rotation); // radians!
	float getRotation() const;

	void setVisible(bool visible);
	bool isVisible() const;

	void setTag(int tag);
	int getTag() const;

	void setLightOccluder(bool occludes);
	bool isLightOccluder() const;

	void markDirty();
	
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void updateTransform() = 0;
	virtual void updateColor() = 0;
	virtual void updateRotation() = 0;
	virtual void reloadGLState();

protected:
	bool m_isDirty = true;
	bool m_isVisible = true;
	bool m_isLightOccluder = false;
	std::unique_ptr<GLProgram> m_glProgram;

	Vec3 m_position;
	Vec2 m_scale = Vec2(1.0f,1.0f);
	Color4 m_color;
	Size m_size;
	float m_rotation = 0.0f;
	int m_tag = -1;
};

//----------------------------------------------------------------------------------------------------------------
// PositionColor
//----------------------------------------------------------------------------------------------------------------

struct PositionColor
{
	Vec3 vertex;
	Color4 color;

	PositionColor(const Vec3& vertex, const Color4& color): vertex(vertex), color(color) {};
	PositionColor() {};
};

//----------------------------------------------------------------------------------------------------------------
// PositionTextureColor
//----------------------------------------------------------------------------------------------------------------

struct PositionTextureColor
{
	Vec3 vertex;
	Vec2 texture;
	Color4 color;

	PositionTextureColor(const Vec3& vertex, const Vec2& texture, const Color4& color): vertex(vertex), texture(texture), color(color) {};
	PositionTextureColor() {};
};