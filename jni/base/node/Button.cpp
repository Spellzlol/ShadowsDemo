#include "Button.h"
#include "TextureCache.h"
#include "utils.h"

Button* Button::create(const std::string& name, const Vec3& position, const std::function<void()> callback, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */)
{
	auto texture = TextureCache::get()->getTexture(name);
	if (not texture)
	{
		return nullptr;
	}

	auto button = new (std::nothrow) Button(position, texture, callback, color);
	if (button)
	{
		button->m_textureName = name;
	}

	return button;
}

Button::Button(const Vec3& position, Texture* texture, const std::function<void()> callback, const Color4& color /* = Color4(1.0f,1.0f,1.0f,1.0f) */):
	Sprite(position,texture,color),
	m_callback(callback)
{
	EventManager::get()->registerListener(this);
}

Button::~Button()
{
	EventManager::get()->unregisterListener(this);
}

void Button::onTouchBegin(const Event& event)
{
	if (isTouchInside(event) and m_isVisible)
	{
		setScale(m_scale.x * m_zoomFactor, m_scale.y * m_zoomFactor);
		m_touchActive = true;
	}
}

void Button::onTouchMove(const Event& event)
{
	if (not m_touchActive)
	{
		return;
	}

	if (not isTouchInside(event))
	{
		setScale(m_scale.x / m_zoomFactor, m_scale.y / m_zoomFactor);
		m_touchActive = false;
	}
}

void Button::onTouchEnd(const Event&)
{
	if (not m_touchActive)
	{
		return;
	}

	m_touchActive = false;
	setScale(m_scale.x / m_zoomFactor, m_scale.y / m_zoomFactor);

	if (m_callback)
	{
		m_callback();
	}

}

bool Button::isTouchInside(const Event& touch)
{
	float x = touch.x;
	float y = SimpleEngine::get()->getHeight() - touch.y;

	float width = m_size.x * m_scale.x;
	float height = m_size.y * m_scale.y;

	return 	(x >= m_position.x - width / 2) and 
			(x <= m_position.x + width / 2) and 
			(y >= m_position.y - height / 2) and 
			(y <= m_position.y + height / 2);
}