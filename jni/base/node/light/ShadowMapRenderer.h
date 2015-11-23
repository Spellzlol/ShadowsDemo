#pragma once

#include "Node.h"
#include <GLES2/gl2.h>

class Texture;
class RenderTexture;
class ShadowMapRenderer : public Node
{
public:
	static ShadowMapRenderer* create(RenderTexture* texture, const Vec3& position, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));
	virtual ~ShadowMapRenderer();

protected:
	ShadowMapRenderer(RenderTexture* texture, const Vec3& position, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

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

	std::unique_ptr<RenderTexture> m_renderTexture;
};