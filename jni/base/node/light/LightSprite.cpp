#include "LightSprite.h"
#include "TextureCache.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "Shaders.h"
#include "SimpleEngine.h"
#include "utils.h"
#include <cmath>

LightSprite* LightSprite::createWithRenderTexture(RenderTexture* renderTexture, const Vec3& position, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */)
{
	auto sprite = new (std::nothrow) LightSprite(position, renderTexture->getTexture(), color);
	if (not sprite)
	{
		return nullptr;
	}
	
	sprite->m_renderTexture.reset(renderTexture);

	return sprite;
}

LightSprite* LightSprite::create(const std::string& name, const Vec3& position, const Color4& color/* = Color4(1.0f,1.0f,1.0f,1.0f)*/)
{
	auto texture = TextureCache::get()->getTexture(name);
	if (not texture)
	{
		return nullptr;
	}

	auto sprite = new (std::nothrow) LightSprite(position, texture, color);
	if (sprite)
	{
		sprite->m_textureName = name;
	}

	return sprite;
}


LightSprite::LightSprite(const Vec3& position, Texture* texture, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */)
{
	m_isLightOccluder = true;

	m_texture = texture;
	int width = m_texture->getWidth();
	int height = m_texture->getHeight();

	m_size = Size(width, height);
	m_position = position;
	m_color = color;

	m_vertexes[0] = PositionTextureColor( Vec3(-width / 2 + position.x,  height / 2 + position.y, position.z), Vec2(0.0f, 0.0f), color);
	m_vertexes[1] = PositionTextureColor( Vec3(-width / 2 + position.x, -height / 2 + position.y, position.z), Vec2(0.0f, 1.0f), color);
	m_vertexes[2] = PositionTextureColor( Vec3( width / 2 + position.x,  height / 2 + position.y, position.z), Vec2(1.0f, 0.0f), color);
	m_vertexes[3] = PositionTextureColor( Vec3( width / 2 + position.x, -height / 2 + position.y, position.z), Vec2(1.0f, 1.0f), color);

	glGenBuffers(1, &m_vbo);

	setGLProgram(PositionTextureColorLightVert, PositionTextureColorLightFrag);
}

void LightSprite::setGLProgram(const char* vShader, const char* fShader)
{
	m_glProgram.reset(GLProgram::create(vShader, fShader));
	m_glProgram->addAttribute(new GLProgramAttribute("a_position", m_aPosition));
	m_glProgram->addAttribute(new GLProgramAttribute("a_color", m_aColor));
	m_glProgram->addAttribute(new GLProgramAttribute("a_texture", m_aTexture));
	m_glProgram->linkProgram();

	m_isDirty = true;
}

LightSprite::~LightSprite()
{
	glDeleteBuffers(1, &m_vbo);
}

void LightSprite::draw()
{
	if (not m_isVisible)
	{
		return;
	}

	glActiveTexture(GL_TEXTURE0);

	m_glProgram->use();

	m_texture->bind();
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
    glEnableVertexAttribArray(m_aTexture);

	glVertexAttribPointer(m_aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, vertex));
	glVertexAttribPointer(m_aTexture, 2, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, texture));
	glVertexAttribPointer(m_aColor, 4, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(m_aPosition);
	glDisableVertexAttribArray(m_aColor);
	glDisableVertexAttribArray(m_aTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_texture->unbind();

#if DEBUG_BUILD == 1
    CHECK_GL_ERROR;
#endif
}

void LightSprite::updateColor()
{
	m_vertexes[0].color = m_color;
	m_vertexes[1].color = m_color;
	m_vertexes[2].color = m_color;
	m_vertexes[3].color = m_color;
}

void LightSprite::updateRotation()
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

void LightSprite::updateTransform()
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

void LightSprite::update(float)
{
}

void LightSprite::reloadGLState()
{
	Node::reloadGLState();

	if (m_renderTexture)
	{
		m_renderTexture->recover();
		m_texture = m_renderTexture->getTexture();
	}
	else if (m_texture)
	{
		m_texture = TextureCache::get()->getTexture(m_textureName);
	}
}