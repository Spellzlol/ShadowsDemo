#include "Scene.h"
#include "utils.h"
#include <algorithm>

Scene::~Scene()
{
	
}

void Scene::setPaused(bool paused)
{
	m_isPaused = paused;
}

void Scene::addObject(Node* newObject)
{
	if (newObject == nullptr)
	{
		LOGE("Trying to add NULL object!");
		return;
	}

	m_objects.emplace_back(newObject);
	std::sort (m_objects.begin(), m_objects.end(), [] (const std::unique_ptr<Node>& first, const std::unique_ptr<Node>& second) -> bool 
	{
		return first->getPosition().z < second->getPosition().z;
	});
}

void Scene::removeObject(Node* oldObject)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (it->get() == oldObject)
		{
			m_objects.erase(it);
			break;
		}
	}
}

void Scene::update(float dt)
{
	if (m_isPaused)
	{
		return;
	}

	if (m_lightEmitter)
	{
		m_lightEmitter->update(dt);
	}

	for (auto& object : m_objects)
	{
		object->update(dt);
	}
}

void Scene::draw()
{
	if (m_lightEmitter)
	{
		m_lightEmitter->draw();
	}

	for (auto& object : m_objects)
	{
		object->draw();
	}
}

void Scene::recover()
{
	if (m_lightEmitter)
	{
		m_lightEmitter->reloadGLState();
	}

	for (auto& object : m_objects)
	{
		object->reloadGLState();
	}
}

const std::vector<std::unique_ptr<Node>>& Scene::getObjects() const
{
	return m_objects;
}

const PointLight* Scene::getLightEmitter() const
{
	if (m_lightEmitter)
	{
		return m_lightEmitter.get();
	}

	return nullptr;
}

void Scene::markDirty()
{
	for (auto& object : m_objects)
	{
		object->markDirty();
	}
}