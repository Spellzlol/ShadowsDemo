#include "Node.h"
#include "SimpleEngine.h"

const Vec3& Node::getPosition() const
{
	return m_position;
}

void Node::setPosition(const Vec3& newPos)
{
	if (m_position != newPos)
	{
		m_position = newPos;
		m_isDirty = true;

		updateTransform();
		updateRotation();
	}
}

void Node::setPosition(float x, float y, float z)
{
	setPosition(Vec3(x,y,z));
}

const Vec2& Node::getScale() const
{
	return m_scale;
}
	
void Node::setScale(const Vec2& scale)
{
	if (m_scale != scale)
	{
		m_scale = scale;
		m_isDirty = true;

		updateTransform();
		updateRotation();
	}
}

void Node::setScale(float x, float y)
{
	setScale(Vec2(x,y));
}

void Node::setSize(const Vec2& size)
{
	if (m_size != size)
	{
		m_size = size;
		m_isDirty = true;

		updateTransform();
		updateRotation();
	}
}

const Size& Node::getSize() const
{
	return m_size;
}

void Node::setColor(const Color4& color)
{
	if (m_color != color)
	{
		m_color = color;
		m_isDirty = true;
		updateColor();
	}
}

const Color4& Node::getColor() const
{
	return m_color;
}

void Node::setRotation(float rotation)
{
	if (m_rotation != rotation)
	{
		m_rotation = rotation;
		m_isDirty = true;
		updateTransform();
		updateRotation();
	}
}

float Node::getRotation() const
{
	return m_rotation;
}

void Node::setVisible(bool visible)
{
	if (m_isVisible != visible)
	{
		m_isVisible = visible;
		m_isDirty = true;
	}
}

bool Node::isVisible() const
{
	return m_isVisible;
}

void Node::reloadGLState()
{
	m_isDirty = true;

	if (m_glProgram)
	{
		m_glProgram->recover();
	}
}

void Node::setTag(int tag)
{
	m_tag = tag;
}

int Node::getTag() const
{
	return m_tag;
}

void Node::setLightOccluder(bool occludes)
{
	if (m_isLightOccluder != occludes)
	{
		m_isLightOccluder = occludes;
		m_isDirty = true;
	}
}

bool Node::isLightOccluder() const
{
	return m_isLightOccluder;
}

void Node::markDirty()
{
	m_isDirty = true;
}