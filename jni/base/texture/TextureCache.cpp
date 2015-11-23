#include "TextureCache.h"
#include "Texture.h"
#include "utils.h"

TextureCache* TextureCache::m_instance = nullptr;

TextureCache* TextureCache::get()
{
	if (m_instance == nullptr)
	{
		m_instance = new TextureCache();
	}

	return m_instance;
}

Texture* TextureCache::getTexture(const std::string& name) const
{
	const auto& it = m_textures.find(name);  
	if (it == m_textures.end())
	{
		return nullptr;
	}

	return it->second.get();
}

void TextureCache::loadTexture(const std::string& name)
{
	auto texture = Texture::create(name);
	if (not texture)
	{
		LOGE("Failed to load texture with name: %s", name.c_str());
		return;
	}

	m_textures[name].reset(texture);
}

void TextureCache::reloadAll()
{
	std::vector<std::string> names;
	names.reserve(m_textures.size());

	for (auto& texture : m_textures)
	{
		names.push_back(texture.first);
	}

	m_textures.clear();

	for (const auto& name : names)
	{
		loadTexture(name);
	}
}