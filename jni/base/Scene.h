#pragma once

#include <vector>
#include <memory>
#include "PointLight.h"

class Scene
{
public:
	virtual ~Scene();

	void addObject(Node* object); // O(n log(n)), после добавления сцена сортируется по Z
	void removeObject(Node* object);
	void draw();
	void recover();
	void setPaused(bool paused);
	void markDirty();

	virtual void update(float dt);

	const std::vector<std::unique_ptr<Node>>& getObjects() const;
	const PointLight* getLightEmitter() const;

protected:
	std::unique_ptr<PointLight> m_lightEmitter;
	std::vector<std::unique_ptr<Node>> m_objects;

	bool m_isPaused = false;
};