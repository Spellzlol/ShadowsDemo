#include "EventManager.h"
#include "utils.h"
#include <algorithm>

EventManager* EventManager::m_instance = nullptr;

EventManager* EventManager::get()
{
	if (m_instance == nullptr)
	{
		m_instance = new EventManager();
	}

	return m_instance;
}

void EventManager::touchBegin(int id, float x, float y)
{
	m_events[id] = Event(id, x, y);

	for (const auto listener : m_listeners)
	{
		listener->onTouchBegin(m_events[id]);
	}
}

void EventManager::touchMove(int id, float x, float y)
{
	const auto& it = m_events.find(id);
	if (it == m_events.end())
	{
		LOGE("Trying to move unknown event! id: %d", id);
		return;
	}

	it->second.x = x;
	it->second.y = y;

	for (const auto listener : m_listeners)
	{
		listener->onTouchMove(it->second);
	}
}

void EventManager::touchEnd(int id, float x, float y)
{
	const auto& it = m_events.find(id);
	if (it == m_events.end())
	{
		LOGE("Trying to end unknown event! id: %d", id);
		return;
	}

	it->second.x = x;
	it->second.y = y;

	for (const auto listener : m_listeners)
	{
		listener->onTouchEnd(it->second);
	}

	m_events.erase(it);
}

void EventManager::registerListener(EventListener* listener)
{
	m_listeners.push_back(listener);
}

void EventManager::unregisterListener(EventListener* listener)
{
	const auto& it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it == m_listeners.end())
	{
		LOGE("Trying to unregister unknown listener!");
		return;
	}

	m_listeners.erase(it);
}