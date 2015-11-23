#pragma once

#include "SimpleEngine.h"
#include "Texture.h"

class RenderTexture
{
public:
	RenderTexture(int width, int height);
	~RenderTexture();

public:
	void begin();
	void end();

	void recover();
	void translate(const Vec2& offset);

	Texture* getTexture() const;

private:
	GLuint m_frameBufferId = 0;
	std::unique_ptr<Texture> m_texture;

	Vec2 m_offset;
};