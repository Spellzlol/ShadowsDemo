#pragma once

#include "JniHelpers.h"
#include <GLES2/gl2.h>

class RenderTexture;
class Texture
{

friend RenderTexture;

public:
	static Texture* create(const std::string& filename);
	static Texture* create(int width, int height, const GLubyte* pixels = nullptr);

protected:
	Texture(const int* pixels, int width, int height);
	Texture(const GLubyte* pixels, int width, int height);

	void init(const GLubyte* pixels, int width, int height);

public:
	~Texture();

	int getWidth() const;
	int getHeight() const;

	void bind() const;
	void unbind() const;

	void enableAliasTexture(bool enable);

private:
	GLuint m_textureId = 0;
	int m_width = 0;
	int m_height = 0;
	bool m_aliasEnabled = true;
};