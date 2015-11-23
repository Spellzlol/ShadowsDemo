#pragma once

#include "SimpleEngine.h"

//----------------------------------------------------------------------------------------------------------------
// Event
//----------------------------------------------------------------------------------------------------------------

class Event
{
public:
	int id = -1;
	float x = 0.0f;
	float y = 0.0f;

	Event(): id(-1), x(0.0f), y(0.0f) {};
	Event(int id, float x, float y): id(id), x(x), y(y) {};
};

//----------------------------------------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------------------------------------

class EventListener
{
public:
	virtual void onTouchBegin(const Event& event) = 0;
	virtual void onTouchMove(const Event& event) = 0;
	virtual void onTouchEnd(const Event& event) = 0;
};

//----------------------------------------------------------------------------------------------------------------
// EventManager
//----------------------------------------------------------------------------------------------------------------

class EventManager
{
public:
	static EventManager* get();

private:
	static EventManager* m_instance;
	EventManager() = default;

public:
	void touchBegin(int id, float x, float y);
	void touchMove(int id, float x, float y);
	void touchEnd(int id, float x, float y);

	void registerListener(EventListener* listener);
	void unregisterListener(EventListener* listener);

private:
	std::map<int, Event> m_events;
	std::vector<EventListener*> m_listeners; // weak
};