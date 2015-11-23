#include "SimpleEngine.h"
#include "JniHelpers.h"
#include "TextureCache.h"
#include "utils.h"
#include "GameScene.h"
#include <GLES2/gl2.h>
#include "GLConfig.h"
#include "EventManager.h"
#include "AsyncFIFO.h"
#include "RenderTexture.h"
#include <algorithm>

SimpleEngine* SimpleEngine::m_instance = nullptr;

void SimpleEngine::create()
{
	Assert(m_instance == nullptr, "Double init of game engine!");

	JniHelpers::get();
	m_instance = new SimpleEngine();
}

SimpleEngine* SimpleEngine::get()
{
	return m_instance;
}

int SimpleEngine::getWidth() const
{
	return m_screenWidth;
}
int SimpleEngine::getHeight() const
{
	return m_screenHeight;
}

const Mat4* SimpleEngine::getProjectionMatrix() const
{
	return m_projectionMatrix.get();
}

void SimpleEngine::setProjectionMatrix(std::unique_ptr<Mat4> matrix)
{
	m_projectionMatrix = std::move(matrix);
	if (m_activeScene)
	{
		m_activeScene->markDirty(); // reload matrix for all shaders
	}
}

void SimpleEngine::restoreDefaultMatrix()
{
	setProjectionMatrix(std::unique_ptr<Mat4>(Mat4::createOrthographic( m_screenWidth, m_screenHeight, -100, 100)));
}

void SimpleEngine::restoreDefaultClearColor()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void SimpleEngine::init(int width, int height)
{
	// activity restore check
	if (m_running)
	{
		reloadGLState();
		return;
	}

	// engine inits
	GLConfig::get();
	TextureCache::get();
	TextureCache::get()->loadTexture("font.png");
	EventManager::get();
	AsyncFIFO::get();

	// opengl inits
	glViewport(0, 0, width, height);
	updateGLSettings();

	m_screenWidth = width;
	m_screenHeight = height;

	restoreDefaultMatrix();

	m_lastFrameTime = utils::systemTime();

	// XXX move somewhere else!
	auto scene = new GameScene();
	setScene(scene);

	m_running = true;
}

void SimpleEngine::setScene(Scene* newScene)
{
	m_nextScene = newScene;
}

const Scene* SimpleEngine::getActiveScene() const
{
	if (m_activeScene)
	{
		return m_activeScene.get();
	}
	else if (m_nextScene)
	{
		return m_nextScene;
	}

	return nullptr;
}


void SimpleEngine::reloadGLState()
{
	TextureCache::get()->reloadAll();

	if (m_activeScene)
	{
		m_activeScene->recover();
	}

	if(m_nextScene)
	{
		m_nextScene->recover();
	}

	updateGLSettings();
}

void SimpleEngine::pause()
{
	m_paused = true;
}

void SimpleEngine::resume()
{
	m_paused = false;
}

void SimpleEngine::mainLoop()
{
	AsyncFIFO::get()->executeAll();

	if (m_paused)
	{
		return;
	}

	if (m_nextScene != nullptr)
	{
		m_activeScene.reset(m_nextScene);
		m_nextScene = nullptr;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	long time = utils::systemTime();
	long diff = time - m_lastFrameTime;
	float dt = diff / 1000.0f;
	m_lastFrameTime = time;

	static int i = 0;
	static float passed = 0.0f;

	passed += dt;
	i++;
	if (passed > 1.0f)
	{
		passed = 0.0f;
		LOGI("Frames: %d", i);
		i = 0;
	}

	for (auto it : m_listeners)
	{
		it->update(dt);
	}

	if (m_activeScene)
	{
		m_activeScene->update(dt);
		m_activeScene->draw();
	}
}

void SimpleEngine::addUpdateListener(UpdateListener* listener)
{
	m_listeners.push_back(listener);
}

void SimpleEngine::removeUpdateListener(UpdateListener* listener)
{
	const auto& it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it == m_listeners.end())
	{
		LOGE("Trying to remove unknown listener!");
		return;
	}

	m_listeners.erase(it);
}

void SimpleEngine::updateGLSettings()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // same for all nodes
	restoreDefaultClearColor();
}