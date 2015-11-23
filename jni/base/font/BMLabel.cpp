#include "BMLabel.h"
#include "FontConfig.h"
#include "utils.h"
#include "Texture.h"
#include "Shaders.h"
#include "TextureCache.h"
#include <cmath>

FontConfig* BMLabel::m_fontConfig = nullptr;

BMLabel* BMLabel::create(const std::string& string, const Vec3& position, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f)*/)
{
	auto label = new (std::nothrow) BMLabel(position, color);
	if (label)
	{
		label->setString(string);
	}

	return label;
}

BMLabel::~BMLabel()
{
	glDeleteBuffers(1, &m_vbo);

	if (m_vertexes)
	{
		delete[] m_vertexes;	
	}
}

BMLabel::BMLabel(const Vec3& position, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f)*/)
{
	m_position = position;
	m_color = color;

	m_textureName = "font.png";
	m_texture = TextureCache::get()->getTexture(m_textureName);

	if (not m_fontConfig)
	{
		m_fontConfig = new (std::nothrow) FontConfig();
		m_fontConfig->load();
	}

	glGenBuffers(1, &m_vbo);

	m_glProgram.reset(GLProgram::create(PositionTextureColorVert, PositionTextureColorFrag));
	m_glProgram->addAttribute(new GLProgramAttribute("a_position", m_aPosition));
	m_glProgram->addAttribute(new GLProgramAttribute("a_color", m_aColor));
	m_glProgram->addAttribute(new GLProgramAttribute("a_texture", m_aTexture));
	m_glProgram->linkProgram();
}

void BMLabel::update(float)
{
}

void BMLabel::draw()
{
	if (m_string.empty() or not m_isVisible)
	{
		return;
	}

	glActiveTexture(GL_TEXTURE0);

	m_glProgram->use();

	m_texture->bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	if (m_isDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(PositionTextureColor)* m_vertexCount, m_vertexes, GL_STREAM_DRAW);

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

    glEnableVertexAttribArray(m_aPosition);
    glEnableVertexAttribArray(m_aColor);
    glEnableVertexAttribArray(m_aTexture);

	glVertexAttribPointer(m_aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, vertex));
	glVertexAttribPointer(m_aTexture, 2, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, texture));
	glVertexAttribPointer(m_aColor, 4, GL_FLOAT, GL_FALSE, sizeof(PositionTextureColor), (GLvoid*) offsetof(struct PositionTextureColor, color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);

	glDisableVertexAttribArray(m_aPosition);
	glDisableVertexAttribArray(m_aColor);
	glDisableVertexAttribArray(m_aTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_texture->unbind();

#if DEBUG_BUILD == 1
    CHECK_GL_ERROR;
#endif
}

void BMLabel::updateColor()
{
	for (size_t i = 0; i < m_vertexCount; ++i)
	{
		m_vertexes[i].color = m_color;
	}
}

void BMLabel::updateRotation()
{
	if (m_rotation == 0.0f or m_string.empty())
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

void BMLabel::updateTransform()
{
	if (m_vertexes)
	{
		delete[] m_vertexes;
		m_vertexes = nullptr;
	}

	m_isDirty = true;

	if (m_string.empty())
	{
		m_vertexCount = 0;
		return;
	}

	float totalWidth = 0;
	float totalHeight = 0;

	for (auto it = m_string.begin(); it != m_string.end(); ++it)
	{
		const auto& letter = m_fontConfig->findLetter(*it);
		if (letter.id == 0)
		{
			LOGE("No letter with char id: %c found!", *it);
			continue;
		}

		totalWidth += letter.width;
		totalHeight = std::max<int>(totalHeight, letter.height);
	}
	
	int textureWidth = m_texture->getWidth() ;
	int textureHeight = m_texture->getHeight();

	totalWidth *= textureWidth * m_scale.x;
	totalHeight *= textureHeight * m_scale.y;

	m_size = Size(totalWidth, totalHeight);

	// now we know label dimensions, start filling the buffer
	m_vertexCount = m_string.size() * 4;
	m_vertexes = new (std::nothrow) PositionTextureColor[m_vertexCount];

	float startPos = m_position.x - totalWidth / 2;

	for (size_t i = 0; i < m_string.size(); ++i)
	{
		const char it = m_string[i];
		const auto& letter = m_fontConfig->findLetter(it);
		if (letter.id == 0)
		{
			LOGE("No letter with char id: %c found!", it);
			continue;
		}

		int j = i*4;
		float height = letter.height * textureHeight * m_scale.x ;
		float width = letter.width * textureWidth * m_scale.y;

		m_vertexes[j].vertex = Vec3(startPos, height + m_position.y, m_position.z);
		m_vertexes[j].texture = Vec2(letter.x, letter.y);
		m_vertexes[j].color = m_color;

		m_vertexes[j+1].vertex = Vec3(startPos, m_position.y, m_position.z);
		m_vertexes[j+1].texture = Vec2(letter.x, letter.y + letter.height);
		m_vertexes[j+1].color = m_color;

		m_vertexes[j+2].vertex = Vec3( width + startPos,  height + m_position.y, m_position.z);
		m_vertexes[j+2].texture = Vec2(letter.x + letter.width, letter.y);
		m_vertexes[j+2].color = m_color;

		m_vertexes[j+3].vertex = Vec3( width + startPos, m_position.y, m_position.z);
		m_vertexes[j+3].texture = Vec2(letter.x + letter.width, letter.y + letter.height);
		m_vertexes[j+3].color = m_color;

		startPos += width;
	}
}

void BMLabel::setString(const std::string& string)
{
	if (m_string == string)
	{
		return;
	}

	m_string = string;

	updateTransform();
	updateRotation();
}

void BMLabel::reloadGLState()
{
	Node::reloadGLState();
	if (m_texture)
	{
		m_texture = TextureCache::get()->getTexture(m_textureName);
	}
}
