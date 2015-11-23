#include "RectangleNode.h"
#include "SimpleEngine.h"
#include "utils.h"

RectangleNode* RectangleNode::create(const Vec3& position, const Size& size, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */)
{
	PositionColor array[4] = 	{ 	
									PositionColor( Vec3(-size.x / 2 + position.x,  size.y / 2 + position.y, position.z), color),
									PositionColor( Vec3(-size.x / 2 + position.x, -size.y / 2 + position.y, position.z), color),
									PositionColor( Vec3( size.x / 2 + position.x,  size.y / 2 + position.y, position.z), color),
									PositionColor( Vec3( size.x / 2 + position.x, -size.y / 2 + position.y, position.z), color)
								};

	auto node = new (std::nothrow) RectangleNode(array, 4);
	if (not node)
	{
		return nullptr;
	}

	node->m_position = position;
	node->m_size = size;
	node->m_color = color;

	return node;
}

RectangleNode::RectangleNode(PositionColor* vertexes, size_t vertexCount): 
	PrimitiveNode(vertexes, vertexCount)
{
	
}

void RectangleNode::updateTransform()
{
	m_vertexes[0].vertex.x = m_scale.x * -m_size.x / 2 + m_position.x;
	m_vertexes[0].vertex.y = m_scale.y *  m_size.y / 2 + m_position.y;
	m_vertexes[0].vertex.z = m_position.z;

	m_vertexes[1].vertex.x = m_scale.x * -m_size.x / 2 + m_position.x;
	m_vertexes[1].vertex.y = m_scale.y * -m_size.y / 2 + m_position.y;
	m_vertexes[1].vertex.z = m_position.z;

	m_vertexes[2].vertex.x = m_scale.x *  m_size.x / 2 + m_position.x;
	m_vertexes[2].vertex.y = m_scale.y *  m_size.y / 2 + m_position.y;
	m_vertexes[2].vertex.z = m_position.z;

	m_vertexes[3].vertex.x = m_scale.x *  m_size.x / 2 + m_position.x;
	m_vertexes[3].vertex.y = m_scale.y * -m_size.y / 2 + m_position.y;
	m_vertexes[3].vertex.z = m_position.z;
}

void RectangleNode::update(float)
{
}