#pragma once

#include <memory>
#include "Scene.h"
#include "Geometry.h"

// ----------------------------------------------------------------------------------------------------------------------------------
// UpdateListener
// ----------------------------------------------------------------------------------------------------------------------------------

class UpdateListener
{
public:
	virtual void update(float dt) = 0;
};

// ----------------------------------------------------------------------------------------------------------------------------------
// SimpleEngine
// ----------------------------------------------------------------------------------------------------------------------------------

class JniHelpers;
class SimpleEngine
{

public:
	static void create();
	static SimpleEngine* get();

	void init(int width, int height);
	void mainLoop();
	
	void setScene(Scene* newScene);
	const Scene* getActiveScene() const;

	void addUpdateListener(UpdateListener* listener);
	void removeUpdateListener(UpdateListener* listener);

	void pause();
	void resume();

	int getWidth() const;
	int getHeight() const;

	const Mat4* getProjectionMatrix() const;
	void setProjectionMatrix(std::unique_ptr<Mat4> matrix);
	void restoreDefaultMatrix();
	void restoreDefaultClearColor();

	void reloadGLState();

private:
	SimpleEngine() = default;
	SimpleEngine(const SimpleEngine& rhs) = delete;
	SimpleEngine(SimpleEngine&& rhs) = delete;

	void updateGLSettings();

private:
	static SimpleEngine* m_instance;

	std::vector<UpdateListener*> m_listeners;

	Scene* m_nextScene = nullptr; // weak
	std::unique_ptr<Scene> m_activeScene;

	float m_screenWidth = 0.0f;
	float m_screenHeight = 0.0f;

	long m_lastFrameTime = 0;

	std::unique_ptr<Mat4> m_projectionMatrix;

	bool m_running = false;
	bool m_paused = false;
};