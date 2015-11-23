#pragma once

#include "EventManager.h"
#include "Sprite.h"

class Button : public Sprite, public EventListener
{
public:
	static Button* create(const std::string& name, const Vec3& position, const std::function<void()> callback, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));
	virtual ~Button();

private:
	Button(const Vec3& position, Texture* texture, const std::function<void()> callback, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

protected:
	virtual void onTouchBegin(const Event& event) override;
	virtual void onTouchMove(const Event& event) override;
	virtual void onTouchEnd(const Event& event) override;

	bool isTouchInside(const Event& touch);

private:
	std::function<void()> m_callback = nullptr;
	const float m_zoomFactor = 1.5f;
	bool m_touchActive = false; // no multitouch on a single button!
};