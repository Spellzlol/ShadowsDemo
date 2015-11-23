#include "PointLight.h"
#include "Geometry.h"
#include "SimpleEngine.h"
#include "RenderTexture.h"
#include "Shaders.h"
#include "utils.h"

#include <cmath>

PointLight::PointLight(const Vec2& position, float size)
{
	m_position = Vec3(position.x, position.y, 0);
	m_size = Size(size, size);

	m_occlusionMap = new (std::nothrow) RenderTexture(size, size);
	m_shadowMap = new (std::nothrow) RenderTexture(size, 1);
	m_lightMap = new (std::nothrow) RenderTexture(size, size);

	m_shadowMapRenderer.reset(ShadowMapRenderer::create(m_occlusionMap, Vec3(size / 2, 1, 0)));
	m_shadowRenderer.reset(ShadowRenderer::create(m_lightMap, m_shadowMap, Vec3(size / 2, size / 2, 0), size));

	m_finalSprite.reset(Sprite::createWithRenderTexture(m_lightMap, Vec3(position.x, position.y, 0)));
	m_finalSprite->setScale(-1.0f, -1.0f);

	updateTransform();
}

void PointLight::updateTransform()
{
	if (m_occlusionMap)
	{
		m_occlusionMap->translate(Vec2(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2));
	}

	if (m_finalSprite)
	{
		m_finalSprite->setPosition(m_position);
	}
}

void PointLight::updateColor()
{
	if (m_shadowRenderer)
	{
		m_shadowRenderer->setColor(m_color);
	}
}

void PointLight::reloadGLState()
{
	Node::reloadGLState();

	if (m_shadowMapRenderer)
	{
		m_shadowMapRenderer->reloadGLState();
	}

	if (m_shadowRenderer)
	{
		m_shadowRenderer->reloadGLState();
	}

	if (m_finalSprite)
	{
		m_finalSprite->reloadGLState();
	}
}

void PointLight::draw()
{
	// 1. OcclusionMap

	m_occlusionMap->begin();
	glDisable(GL_BLEND);

	const auto& objects = SimpleEngine::get()->getActiveScene()->getObjects();
	for (const auto& object : objects)
	{
		if (object->isLightOccluder() and isColliding(object.get()))
		{
			object->draw();
		}
	}

	m_occlusionMap->end();

	// 2. ShadowMap

	 m_shadowMap->begin();

	 m_shadowMapRenderer->draw();

	 m_shadowMap->end();

	 glEnable(GL_BLEND);

	// 3. Render shadows

	m_lightMap->begin();

	m_shadowRenderer->draw();

	m_lightMap->end();

	// 4. Out

	m_finalSprite->draw();
}

bool PointLight::isColliding(const Node* object) const
{
	float mRadius = sqrt(pow(m_size.x/2, 2) + pow(m_size.y/2,2));
	float otherRadius = sqrt(pow(object->getSize().x/2, 2) + pow(object->getSize().y/2,2));

	float distance = sqrt(pow(m_position.x - object->getPosition().x,2) + pow(m_position.y - object->getPosition().y, 2));

	return distance <= mRadius + otherRadius;
}