#include "GameLight.h"

GameLight::GameLight(const Vec2& position, float size):
	PointLight(position,size)
{

}

void GameLight::update(float dt)
{
	float scale = 1.0f;// 1920.0f / 1280.0f;

	float pos_x = m_position.x;
	float pos_y = m_position.y;

	if (pos_y  < 130*scale)
	{
		pos_y = 131*scale;
		m_directionY = 0;
		m_directionX = 1;
	}

	if (pos_x > 590*scale)
	{
		pos_x = 589*scale;
		m_directionY = 1;
		m_directionX = 0;
	}

	if (pos_y > 1150*scale)
	{
		pos_y = 1149*scale;
		m_directionY = 0;
		m_directionX = -1;
	}

	if (pos_x < 130*scale)
	{
		pos_x = 131*scale;
		m_directionX = 0;
		m_directionY = -1;
	}

	setPosition(pos_x + 100 * dt * m_directionX, pos_y + 100 * dt * m_directionY, 0);
}
