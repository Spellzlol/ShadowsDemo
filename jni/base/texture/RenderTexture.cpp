#include "RenderTexture.h"
#include "Texture.h"
#include "utils.h"

RenderTexture::~RenderTexture()
{
	glDeleteFramebuffers(1, &m_frameBufferId);
}

RenderTexture::RenderTexture(int width, int height)
{
	m_texture.reset(Texture::create(width, height));

	glGenFramebuffers(1, &m_frameBufferId);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->m_textureId, 0);

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		glClearColor(0.0f,0.0f,0.0f,1.0f);
	}
	else
	{
		LOGE("RenderTexture failed to complete framebuffer! Status: %d", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECK_GL_ERROR;
}

void RenderTexture::translate(const Vec2& offset)
{
	m_offset = offset;
}

void RenderTexture::begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glClear(GL_COLOR_BUFFER_BIT);

	if (m_offset.x != 0 or m_offset.y != 0)
	{
		float screenWidth = SimpleEngine::get()->getWidth();
		float screenHeight = SimpleEngine::get()->getHeight();

		SimpleEngine::get()->setProjectionMatrix(std::unique_ptr<Mat4>(Mat4::createOrthographic( screenWidth + m_offset.x, screenHeight + m_offset.y, 
			-100, 100, m_offset.x, m_offset.y)));
	}
}

void RenderTexture::end()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SimpleEngine::get()->restoreDefaultClearColor();

	if (m_offset.x != 0 or m_offset.y != 0)
	{
		SimpleEngine::get()->restoreDefaultMatrix();
	}

	CHECK_GL_ERROR;
}

Texture* RenderTexture::getTexture() const
{
	return m_texture.get();
}

void RenderTexture::recover()
{
	int width = m_texture->getWidth();
	int height = m_texture->getHeight();

	glGenFramebuffers(1, &m_frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

	m_texture.reset(Texture::create(width, height));

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->m_textureId, 0);

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		glClearColor(0.0f,0.0f,0.0f,1.0f);
	}
	else
	{
		LOGE("RenderTexture failed to complete framebuffer! Status: %d", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SimpleEngine::get()->restoreDefaultClearColor();
	CHECK_GL_ERROR;
}