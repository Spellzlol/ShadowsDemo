#pragma once

#include "SimpleEngine.h"

class Texture;
class TextureCache
{
public:
	static TextureCache* get();

	Texture* getTexture(const std::string& name) const;
	void loadTexture(const std::string& name);
	void reloadAll();

private:
	TextureCache() = default;
	static TextureCache* m_instance;

private:
	std::map<std::string, std::unique_ptr<Texture>> m_textures;
};