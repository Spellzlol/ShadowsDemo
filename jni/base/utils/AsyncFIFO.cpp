#include "AsyncFIFO.h"

AsyncFIFO* AsyncFIFO::m_instance = nullptr;

AsyncFIFO* AsyncFIFO::get()
{
	if (m_instance == nullptr)
	{
		m_instance = new AsyncFIFO();
	}

	return m_instance;
}

void AsyncFIFO::pushBack(std::function<void()> callback)
{
	m_mutex.lock();

	m_callbacks.push_back(callback);

	m_mutex.unlock();
}

void AsyncFIFO::executeAll()
{
	m_mutex.lock();

	for (const auto& function : m_callbacks)
	{
		function();
	}

	m_callbacks.clear();

	m_mutex.unlock();
}