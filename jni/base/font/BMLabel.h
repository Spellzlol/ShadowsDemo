#pragma once

#include "Node.h"
#include <GLES2/gl2.h>

class FontConfig;
class Texture;
class BMLabel : public Node
{
public:
	static BMLabel* create(const std::string& string, const Vec3& position, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));
	virtual ~BMLabel();

private:
	BMLabel(const Vec3& position, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

public:
	virtual void update(float dt) override;
	virtual void draw() override;
	virtual void updateColor() override;
	virtual void updateRotation() override;
	virtual void updateTransform() override;
	virtual void reloadGLState() override;

	void setString(const std::string& string);

private:
	GLuint m_vbo = -1;

	GLuint m_aPosition = 0;
	GLuint m_aColor = 1;
	GLuint m_aTexture = 2;

	std::string m_string;
	static FontConfig* m_fontConfig ;

	PositionTextureColor* m_vertexes = nullptr;
	size_t m_vertexCount = 0;

	std::string m_textureName;
	Texture* m_texture = nullptr; // weak
};