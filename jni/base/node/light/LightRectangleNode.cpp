#include "LightRectangleNode.h"
#include "SimpleEngine.h"
#include "utils.h"
#include "PointLight.h"
#include "Shaders.h"

LightRectangleNode* LightRectangleNode::create(const Vec3& position, const Size& size, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */)
{
	PositionColor array[4] = 	{ 	
									PositionColor( Vec3(-size.x / 2 + position.x,  size.y / 2 + position.y, position.z), color),
									PositionColor( Vec3(-size.x / 2 + position.x, -size.y / 2 + position.y, position.z), color),
									PositionColor( Vec3( size.x / 2 + position.x,  size.y / 2 + position.y, position.z), color),
									PositionColor( Vec3( size.x / 2 + position.x, -size.y / 2 + position.y, position.z), color)
								};

	auto node = new (std::nothrow) LightRectangleNode(array, 4);
	if (not node)
	{
		return nullptr;
	}

	node->m_position = position;
	node->m_size = size;
	node->m_color = color;

	return node;
}

LightRectangleNode::LightRectangleNode(PositionColor* vertexes, size_t vertexCount): 
	PrimitiveNode(vertexes, vertexCount)
{
	m_isLightOccluder = true;

	m_glProgram.reset(GLProgram::create(PositionColorLightVert, PositionColorLightFrag));
	m_glProgram->addAttribute(new GLProgramAttribute("a_position", m_aPosition));
	m_glProgram->addAttribute(new GLProgramAttribute("a_color", m_aColor));
	m_glProgram->linkProgram();
}

void LightRectangleNode::updateTransform()
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

void LightRectangleNode::update(float)
{
}

void LightRectangleNode::draw()
{
	if (not m_isVisible)
	{
		return;
	}

	m_glProgram->use();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	if (m_isDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(PositionColor)* m_vertexCount, m_vertexes, GL_STREAM_DRAW);

		auto projectionPos = m_glProgram->findUniformForName("u_projection");
		if (projectionPos != -1)
		{
			m_glProgram->setUniformMat4(projectionPos, SimpleEngine::get()->getProjectionMatrix());
		}
        
        m_isDirty = false;
    }

    const auto scene = SimpleEngine::get()->getActiveScene();
    const auto emitter = scene->getLightEmitter();
	Assert(emitter != nullptr, "LightRectangleNode attached to scene with no light source!");

    auto lightPos = m_glProgram->findUniformForName("u_lightPos");
	if (lightPos != -1)
	{
		const auto& position = emitter->getPosition();
		m_glProgram->setUniformVec2(lightPos, Vec2(position.x, position.y));
	}

	auto lightSizePos = m_glProgram->findUniformForName("u_lightSize");
	if (lightSizePos != -1)
	{
		float size = emitter->getSize().x;
		m_glProgram->setUniformFloat(lightSizePos, size);
	}

    glEnableVertexAttribArray(m_aPosition);
    glEnableVertexAttribArray(m_aColor);

	glVertexAttribPointer(m_aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(PositionColor), (GLvoid*) offsetof(struct PositionColor, vertex));
	glVertexAttribPointer(m_aColor, 4, GL_FLOAT, GL_FALSE, sizeof(PositionColor), (GLvoid*) offsetof(struct PositionColor, color));

	if(m_useindices and m_indicesCount > 0)
	{
		glDrawElements(GL_TRIANGLE_STRIP, m_indicesCount, GL_UNSIGNED_BYTE, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
	}

	glDisableVertexAttribArray(m_aPosition);
	glDisableVertexAttribArray(m_aColor);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if DEBUG_BUILD == 1
    CHECK_GL_ERROR;
#endif
}
