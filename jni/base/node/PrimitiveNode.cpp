#include "PrimitiveNode.h"
#include "Shaders.h"
#include "GLProgram.h"
#include "utils.h"
#include "SimpleEngine.h"
#include <cmath>

PrimitiveNode::PrimitiveNode(PositionColor* vertexes, size_t vertexCount, GLubyte* indices /* = nullptr */, size_t indicesCount /* = 0 */)
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	m_vertexes = new PositionColor[vertexCount];
	memcpy(m_vertexes, vertexes, sizeof(PositionColor)*vertexCount);
	m_vertexCount = vertexCount;

	if(indices != nullptr and indicesCount > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indicesCount, indices, GL_STATIC_DRAW);

		m_indicesCount = indicesCount;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	m_glProgram.reset(GLProgram::create(PositionColorVert, PositionColorFrag));
	m_glProgram->addAttribute(new GLProgramAttribute("a_position", m_aPosition));
	m_glProgram->addAttribute(new GLProgramAttribute("a_color", m_aColor));
	m_glProgram->linkProgram();
}

PrimitiveNode::~PrimitiveNode()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);

	if (m_vertexes)
	{
		delete[] m_vertexes;
	}
}

void PrimitiveNode::useIndices(bool use)
{
	m_useindices = use;
}

//----------------------------------------------------------------------------------------------------------------
// Color
//----------------------------------------------------------------------------------------------------------------

void PrimitiveNode::updateColor()
{
	m_vertexes[0].color = m_color;
	m_vertexes[1].color = m_color;
	m_vertexes[2].color = m_color;
	m_vertexes[3].color = m_color;
}

//----------------------------------------------------------------------------------------------------------------
// Rotation
//----------------------------------------------------------------------------------------------------------------

void PrimitiveNode::updateRotation()
{
	if (m_rotation == 0.0f)
	{
		return;
	}

	for (size_t i = 0; i < m_vertexCount; ++i)
	{
		float x = m_vertexes[i].vertex.x - m_position.x;
		float y = m_vertexes[i].vertex.y - m_position.y;

		m_vertexes[i].vertex.x = x * cos(m_rotation) - y * sin(m_rotation) + m_position.x;
		m_vertexes[i].vertex.y = x * sin(m_rotation) + y * cos(m_rotation) + m_position.y;
	}
}

//----------------------------------------------------------------------------------------------------------------
// Draw
//----------------------------------------------------------------------------------------------------------------

void PrimitiveNode::draw()
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
