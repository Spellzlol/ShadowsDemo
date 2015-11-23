#include "GLConfig.h"
#include "utils.h"

GLConfig* GLConfig::m_instance = nullptr;

const GLConfig* GLConfig::get()
{
	if (not m_instance)
	{
		m_instance = new GLConfig();
	}
	
	return m_instance;
}

GLConfig::GLConfig()
{
	m_extensions = (char*)glGetString(GL_EXTENSIONS);

	if (m_extensions == nullptr or strlen(m_extensions) == 0)
	{
		LOGI("No extensions found");
		return;
	}

	m_supportsVAO = strstr(m_extensions, "vertex_array_object") != nullptr;
	LOGI("Supports vao: %d", m_supportsVAO ? 1 : 0);

	// TODO: load function pointers from <EGL/egl.h>
	// use eglGetProcAddress
}

bool GLConfig::supportsVAO() const
{
	return m_supportsVAO;
}