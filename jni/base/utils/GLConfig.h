#pragma once

#include "SimpleEngine.h"

class GLConfig 
{
public:
	static const GLConfig* get();

	bool supportsVAO() const;

private:
	GLConfig();
	static GLConfig* m_instance;

private:
	char* m_extensions = nullptr;

	bool m_supportsVAO = false;
};