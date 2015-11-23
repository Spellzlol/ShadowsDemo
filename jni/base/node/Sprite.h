#pragma once

#include "Node.h"
#include <GLES2/gl2.h>

class Texture;
class RenderTexture;
class Sprite : public Node
{
public:
	static Sprite* createWithRenderTexture(RenderTexture* texture, const Vec3& position, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));
	static Sprite* create(const std::string& name, const Vec3& position, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));
	virtual ~Sprite();

	void setGLProgram(const char* vShader, const char* fShader);

protected:
	Sprite(const Vec3& position, Texture* texture, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

public:
	virtual void update(float dt) override;
	virtual void draw() override;
	virtual void updateColor() override;
	virtual void updateRotation() override;
	virtual void updateTransform() override;
	virtual void reloadGLState() override;

protected:
	GLuint m_vbo = -1;

	GLuint m_aPosition = 0;
	GLuint m_aColor = 1;
	GLuint m_aTexture = 2;

	PositionTextureColor m_vertexes[4];

	std::string m_textureName;
	Texture* m_texture = nullptr; // weak
	std::unique_ptr<RenderTexture> m_renderTexture; //strong
};