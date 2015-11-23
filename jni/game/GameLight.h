#pragma once

#include "PointLight.h"

class GameLight : public PointLight
{
public:
	GameLight(const Vec2& position, float size);
	virtual void update(float) override;

private:
	int m_directionX = 0;
	int m_directionY = -1;
};