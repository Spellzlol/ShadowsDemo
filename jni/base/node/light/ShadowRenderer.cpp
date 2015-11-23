#include "ShadowRenderer.h"
#include "TextureCache.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "Shaders.h"
#include "SimpleEngine.h"
#include "utils.h"
#include <cmath>

ShadowRenderer* ShadowRenderer::create(RenderTexture* texture, RenderTexture* shadowMap, const Vec3& position, float lightSize, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */)
{
	auto renderer = new (std::nothrow) ShadowRenderer(texture, shadowMap, position, lightSize, color);
	if (not renderer)
	{
		return nullptr;
	}
	

	return renderer;
}

ShadowRenderer::ShadowRenderer(RenderTexture* texture, RenderTexture* shadowMap, const Vec3& position, float lightSize, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */):
	m_lightSize(lightSize),
	m_renderTexture(texture),
	m_shadowMap(shadowMap)
{
	int width = m_renderTexture->getTexture()->getWidth();
	int height = m_renderTexture->getTexture()->getHeight();

	m_size = Size(width, height);
	m_position = position;
	m_color = color;

	m_vertexes[0] = PositionTextureColor( Vec3(-width / 2 + position.x,  height / 2 + position.y, position.z), Vec2(0.0f, 0.0f), color);
	m_vertexes[1] = PositionTextureColor( Vec3(-width / 2 + position.x, -height / 2 + position.y, position.z), Vec2(0.0f, 1.0f), color);
	m_vertexes[2] = PositionTextureColor( Vec3( width / 2 + position.x,  height / 2 + position.y, position.z), Vec2(1.0f, 0.0f), color);
	m_vertexes[3] = PositionTextureColor( Vec3( width / 2 + position.x, -height / 2 + position.y, position.z), Vec2(1.0f, 1.0f), color);

	glGenBuffers(1, &m_vbo);

	m_glProgram.reset(GLProgram::create(PositionTextureColorVert, ShadowRenderFrag));
	m_glProgram->addAttribute(new GLProgramAttribute("a_position", m_aPosition));
	m_glProgram->addAttribute(new GLProgramAttribute("a_color", m_aColor));
	m_glProgram->addAttribute(new GLProgramAttribute("a_texture", m_aTexture));
	m_glProgram->linkProgram();
}

ShadowRenderer::~ShadowRenderer()
{
	glDeleteBuffers(1, &m_vbo);
}

void ShadowRenderer::draw()
{
	if (not m_isVisible)
	{
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	m_renderTexture->getTexture()->bind();

	glActiveTexture(GL_TEXTURE1);
	m_shadowMap->getTexture()->bind();

	m_glProgram->use();
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	if (m_isDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(PositionTextureColor)* 4, m_vertexes, GL_STREAM_DRAW);

		auto projectionPos = m_glProgram->findUniformForName("u_projection");
		if (projectionPos != -1)
		{
			m_glProgram->setUniformMat4(projectionPos, SimpleEngine::get()->getProjectionMatrix());
		}

		auto texturePos = m_glProgram->findUniformForName("u_texture0");
		if (texturePos != -1)
		{
			m_glProgram->setUniformTexture(texturePos, 0);
		}

		auto texturePosShadow = m_glProgram->findUniformForName("u_texture1");
		if (texturePosShadow != -1)
		{
			m_glProgram->setUniformTexture(texturePosShadow, 1);
		}

		auto lightSizePos = m_glProgram->findUniformForName("u_lightSize");
		if (lightSizePos != -1)
		{
			m_glProgram->setUniformFloat(lightSizePos, m_lightSize);
		}

		auto lightColorPos = m_glProgram->findUniformForName("u_lightColor");
		if (lightColorPos != -1)
		{
			m_glProgram->setUniformVec3(lightColorPos, Vec3(m_color.x, m_color.y, m_color.z));
		}
        
        m_isDirty = false;
    }

    glEnableVertexAttribArray(m_aPosition);
    glEnableVertexAttribArray(m_aColor);
    glEnableVertexAttribArray(m_aTexture);

	glVertexAttribPointer(m_aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, vertex));
	glVertexAttribPointer(m_aTexture, 2, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, texture));
	glVertexAttribPointer(m_aColor, 4, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(m_aPosition);
	glDisableVertexAttribArray(m_aColor);
	glDisableVertexAttribArray(m_aTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_renderTexture->getTexture()->unbind();
    m_shadowMap->getTexture()->unbind();

#if DEBUG_BUILD == 1
    CHECK_GL_ERROR;
#endif
}

void ShadowRenderer::updateColor()
{
	m_vertexes[0].color = m_color;
	m_vertexes[1].color = m_color;
	m_vertexes[2].color = m_color;
	m_vertexes[3].color = m_color;
}

void ShadowRenderer::updateRotation()
{
	if (m_rotation == 0.0f)
	{
		return;
	}

	for (size_t i = 0; i < 4; ++i)
	{
		float x = m_vertexes[i].vertex.x - m_position.x;
		float y = m_vertexes[i].vertex.y - m_position.y;

		m_vertexes[i].vertex.x = x * cos(m_rotation) - y * sin(m_rotation) + m_position.x;
		m_vertexes[i].vertex.y = x * sin(m_rotation) + y * cos(m_rotation) + m_position.y;
	}
}

void ShadowRenderer::updateTransform()
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

void ShadowRenderer::update(float)
{
}

void ShadowRenderer::reloadGLState()
{
	Node::reloadGLState();

	if (m_renderTexture)
	{
		m_renderTexture->recover();
	}
	
	if (m_shadowMap)
	{
		m_shadowMap->recover();
	}
}
