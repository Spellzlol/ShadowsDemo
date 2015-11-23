#pragma once

#include <map>

class LetterConfig
{
public:
	char id = 0;
	float x = 0.0f;
	float y = 0.0f;
	float width = 0.0f;
	float height = 0.0f;

	LetterConfig(char id, float x, float y, float width, float height): id(id), x(x), y(y), width(width), height(height) {};
	LetterConfig() = default;
};

class FontConfig
{
public:
	const LetterConfig& findLetter(char id);
	void load(); // TODO: multiple font support

private:
	std::map<char, LetterConfig> m_letters;
};