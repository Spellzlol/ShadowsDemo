#include "GameScene.h"
#include "SimpleEngine.h"
#include "RectangleNode.h"
#include "RenderTexture.h"
#include "TextureCache.h"
#include "Button.h"
#include "Shaders.h"
#include "GameLight.h"
#include "LightSprite.h"
#include "LightRectangleNode.h"

GameScene::GameScene()
{
	TextureCache::get()->loadTexture("test.png");

	int width = SimpleEngine::get()->getWidth();
	int height = SimpleEngine::get()->getHeight();

	auto node = LightSprite::create("test.png", Vec3(width *0.5f, height * 0.5f, 5));
	addObject(node);

	m_lightEmitter.reset(new GameLight(Vec2(130, 1150), 512));
}
