#pragma once

#include "SimpleEngine.h"
#include <mutex>

class AsyncFIFO
{
public:
	static AsyncFIFO* get();

private:
	static AsyncFIFO* m_instance;
	AsyncFIFO() = default;

public:
	void pushBack(std::function<void()> callback);
	void executeAll();

private:
	std::vector<std::function<void()>> m_callbacks;
	std::mutex m_mutex;
};