#pragma once

#include "Sprite.h"
#include "ShadowRenderer.h"
#include "ShadowMapRenderer.h"
#include <GLES2/gl2.h>

class RenderTexture;
class PointLight : public Node
{
public:
	PointLight(const Vec2& position, float size);

public:
	virtual void draw() override;
	virtual void updateColor() override;
	virtual void updateRotation() override {};
	virtual void update(float) override {};
	virtual void updateTransform() override;
	virtual void reloadGLState() override;

private:
	bool isColliding(const Node* object) const;

private:
	RenderTexture* m_occlusionMap = nullptr;
	RenderTexture* m_shadowMap = nullptr;
	RenderTexture* m_lightMap = nullptr;

	std::unique_ptr<ShadowMapRenderer> m_shadowMapRenderer;
	std::unique_ptr<ShadowRenderer> m_shadowRenderer;
	std::unique_ptr<Sprite> m_finalSprite;
};