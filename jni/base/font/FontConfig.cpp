#include "FontConfig.h"
#include "JniHelpers.h"
#include "utils.h"
#include "Texture.h"
#include "TextureCache.h"

extern jobject m_bitmapLoader;

void FontConfig::load()
{
	if (not m_letters.empty())
	{
		return;
	}

	auto texture = TextureCache::get()->getTexture("font.png");
	float textureWidth = texture->getWidth();
	float textureHeight = texture->getHeight();

	const auto env = JniHelpers::get()->getEnv();

	auto load = JniHelpers::get()->getMethodID("com/gsom/simpleengine/BitmapLoader", "readTextFile", "(Ljava/lang/String;)Ljava/lang/String;");
	auto jname = env->NewStringUTF("font.txt");
	auto jtext = (jstring) env->CallObjectMethod(m_bitmapLoader, load, jname);
    
	auto fileString = JniHelpers::get()->jstring2string(jtext);

	size_t endl = fileString.find('\n');

	// TODO: очень много алокаций, подумать
	while(endl != std::string::npos)
	{
		std::string line = fileString.substr (0,endl);
		fileString.erase(0, line.size()+1);
		endl = fileString.find('\n');

		size_t idStart = line.find('=');
		size_t idFinish = line.find('x');
		int id = utils::toNumber<int>( line.substr (idStart+1, idFinish - idStart - 2) );

		size_t xStart = line.find('x');
		size_t xFinish = line.find('y');
		int x = utils::toNumber<int>( line.substr (xStart+2, xFinish - xStart - 2) );

		size_t yStart = line.find('y');
		size_t yFinish = line.find("width");
		int y = utils::toNumber<int>( line.substr (yStart+2, yFinish - yStart - 2) );

		size_t widthStart = line.find("width");
		size_t widthFinish = line.find("height");
		int width = utils::toNumber<int>( line.substr (widthStart+6, widthFinish - widthStart - 6) );

		size_t heightStart = line.find("height");
		size_t heightFinish = line.find('\n');
		int height = utils::toNumber<int>( line.substr (heightStart+7, heightFinish - heightStart - 7) );

		LetterConfig letter(static_cast<char>(id), x / textureWidth, y / textureHeight, width / textureWidth, height /textureHeight);
		m_letters[letter.id] = letter;

	}

    env->DeleteLocalRef(jname);
    env->DeleteLocalRef(jtext);
}

const LetterConfig& FontConfig::findLetter(char id)
{
	auto it = m_letters.find(id);
	if (it == m_letters.end())
	{
		return std::move(LetterConfig());
	}

	return it->second;
}