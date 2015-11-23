#pragma once

#include "Node.h"
#include <GLES2/gl2.h>

class Texture;
class RenderTexture;
class ShadowRenderer : public Node
{
public:
	static ShadowRenderer* create(RenderTexture* texture, RenderTexture* shadowMap, const Vec3& position, float lightSize, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));
	virtual ~ShadowRenderer();

protected:
	ShadowRenderer(RenderTexture* texture, RenderTexture* shadowMap, const Vec3& position, float lightSize, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

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

	float m_lightSize = 0.0f;

	PositionTextureColor m_vertexes[4];

	std::unique_ptr<RenderTexture> m_renderTexture;
	std::unique_ptr<RenderTexture> m_shadowMap;
};