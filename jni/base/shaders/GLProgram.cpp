#include "GLProgram.h"
#include "utils.h"
#include "Texture.h"

// ----------------------------------------------------------------------------------------------------------------------------------
// GLProgramAttribute
// ----------------------------------------------------------------------------------------------------------------------------------

GLProgramAttribute::GLProgramAttribute(const std::string& name, GLuint position):
	m_name(name),
	m_position(position)
{
}

const std::string& GLProgramAttribute::getName() const
{
	return m_name;
}

GLuint GLProgramAttribute::getPosition() const
{
	return m_position;
}

// ----------------------------------------------------------------------------------------------------------------------------------
// GLProgramUniform
// ----------------------------------------------------------------------------------------------------------------------------------

GLProgramUniform::GLProgramUniform(const std::string& name, GLuint position, GLint size, GLenum type):
	m_name(name),
	m_position(position),
	m_size(size),
	m_type(type)
{
}

const std::string& GLProgramUniform::getName() const
{
	return m_name;
}

GLuint GLProgramUniform::getPosition() const
{
	return m_position;
}

GLint GLProgramUniform::getSize() const
{
	return m_size;
}

GLenum GLProgramUniform::getType() const
{
	return m_type;
}

// ----------------------------------------------------------------------------------------------------------------------------------
// GLProgram
// ----------------------------------------------------------------------------------------------------------------------------------

GLProgram::~GLProgram()
{
	if(m_programObject)
	{
		glDeleteProgram(m_programObject);
	}

	if (m_vertexShader)
	{
		glDeleteShader(m_vertexShader);
	}

	if (m_fragmentShader)
	{
		glDeleteShader(m_fragmentShader);
	}
}

GLProgram* GLProgram::create(const char* vShader, const char* fShader)
{
	auto vertexShader = loadAndCompileShader(vShader, GL_VERTEX_SHADER);
	auto fragmentShader = loadAndCompileShader(fShader, GL_FRAGMENT_SHADER);

	if (vertexShader == 0 or fragmentShader == 0)
	{
		LOGE("Failed to load and compile shaders");
		return nullptr;
	}

	GLProgram* program = new (std::nothrow) GLProgram();
	
	if (not program)
	{
		LOGE("Failed to create GLProgram");
		return nullptr;
	}

	program->m_programObject = glCreateProgram();
	if (program->m_programObject == 0)
	{
		CHECK_GL_ERROR;

		delete program;
		LOGE("Failed to create program object");
		return nullptr;
	}

	glAttachShader(program->m_programObject, vertexShader);
	glAttachShader(program->m_programObject, fragmentShader);

	program->m_vertexShader = vertexShader;
	program->m_fragmentShader = fragmentShader;

	program->m_vShader = vShader;
	program->m_fShader = fShader;

	return program;
}

GLuint GLProgram::loadAndCompileShader(const char* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0)
	{
		CHECK_GL_ERROR;
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled)
	{
		return shader;
	}

	CHECK_GL_ERROR;

	GLint length = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length > 1)
	{
		GLchar* message = new GLchar(sizeof(GLchar)*length);
		glGetShaderInfoLog(shader, length, NULL, message);

		LOGE("Shader compiling error output: %s", message);
		delete message;
	}

	glDeleteShader(shader);
	return 0;
}

void GLProgram::addAttribute(GLProgramAttribute *attribute)
{
	glBindAttribLocation(m_programObject, attribute->getPosition(), attribute->getName().c_str());
	m_attributes[attribute->getPosition()].reset(attribute);
}
	
void GLProgram::linkProgram()
{
	glLinkProgram(m_programObject);

	GLint linked = 0;
	glGetProgramiv(m_programObject, GL_LINK_STATUS, &linked);

	if (linked)
	{
		parseUniforms();
		return;
	}

	CHECK_GL_ERROR;

	GLint length = 0;
	glGetProgramiv(m_programObject, GL_INFO_LOG_LENGTH, &length);
	if (length > 1)
	{
		GLchar* message = new GLchar(sizeof(GLchar) * length);
		glGetProgramInfoLog(m_programObject, length, NULL, message);

		LOGE("Shader linking error output: %s", message);
		delete message;
	}
}

void GLProgram::parseUniforms()
{
	GLint activeUniforms;
    glGetProgramiv(m_programObject, GL_ACTIVE_UNIFORMS, &activeUniforms);

    if(activeUniforms <= 0)
    {
    	return;
    }

	GLint length;
    glGetProgramiv(m_programObject, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);

    if(length <= 0)
    {
    	return;
    }

    GLchar* uniformName = new GLchar[length];
    GLint size = 0;
    GLenum type = GL_FLOAT;

    for(int i = 0; i < activeUniforms; ++i)
    {
        glGetActiveUniform(m_programObject, i, length, NULL, &size, &type, uniformName);
        GLint location = glGetUniformLocation(m_programObject, uniformName);

        if (location == -1)
        {
        	LOGI("Uniform %s is not active", uniformName);
        	continue;
        }

        m_uniforms[location].reset(new GLProgramUniform(uniformName, location, size, type));

        GLenum error = glGetError(); 
        if (error != GL_NO_ERROR) 
        { 
            LOGE("Error fetching uniform: %d, name: %s", error, uniformName);
            m_uniforms[location].reset();
        }
    }

    delete[] uniformName;
}

void GLProgram::recover()
{
	m_vertexShader = loadAndCompileShader(m_vShader, GL_VERTEX_SHADER);
	m_fragmentShader = loadAndCompileShader(m_fShader, GL_FRAGMENT_SHADER);

	if (m_vertexShader == 0 or m_fragmentShader == 0)
	{
		return;
	}

	m_programObject = glCreateProgram();
	if (m_programObject == 0)
	{
		CHECK_GL_ERROR;
		return;
	}

	glAttachShader(m_programObject, m_vertexShader);
	glAttachShader(m_programObject, m_fragmentShader);

	for (const auto& it : m_attributes)
	{
		glBindAttribLocation(m_programObject, it.second->getPosition(), it.second->getName().c_str());
	}

	linkProgram();
}

void GLProgram::use()
{
	if (glIsProgram(m_programObject) != GL_TRUE)
	{
		recover();
	}

	glUseProgram(m_programObject);
}

GLint GLProgram::findUniformForName(const std::string& name)
{
	for (const auto& uniform : m_uniforms)
	{
		if (uniform.second->getName() == name)
		{
			return uniform.first;
		}
	}

	return -1;
}

void GLProgram::setUniformVec4(GLuint position, const Vec4& data)
{
	auto uniform = m_uniforms.find(position);
	if (uniform == m_uniforms.end())
	{
		LOGE("Not uniform found for position: %u", position);
		return;
	}

	if (uniform->second->getType() != GL_FLOAT_VEC4 or uniform->second->getSize() != 1)
	{
		LOGE("Incorrect type or size uniform, want Vec4, have: %d, size: %d", (int)uniform->second->getType(), (int)uniform->second->getSize());
		return;
	}

	glUniform4f(position, data.x, data.y, data.z, data.w);

	CHECK_GL_ERROR;
}

void GLProgram::setUniformVec3(GLuint position, const Vec3& data)
{
	auto uniform = m_uniforms.find(position);
	if (uniform == m_uniforms.end())
	{
		LOGE("Not uniform found for position: %u", position);
		return;
	}

	if (uniform->second->getType() != GL_FLOAT_VEC3 or uniform->second->getSize() != 1)
	{
		LOGE("Incorrect type or size uniform, want Vec3, have: %d, size: %d", (int)uniform->second->getType(), (int)uniform->second->getSize());
		return;
	}

	glUniform3f(position, data.x, data.y, data.z);

	CHECK_GL_ERROR;
}

void GLProgram::setUniformMat4(GLuint position, const Mat4* data)
{
	auto uniform = m_uniforms.find(position);
	if (uniform == m_uniforms.end())
	{
		LOGE("Not uniform found for position: %u", position);
		return;
	}

	if (uniform->second->getType() != GL_FLOAT_MAT4 or uniform->second->getSize() != 1)
	{
		LOGE("Incorrect type or size uniform, want Mat4, have: %d, size: %d", (int)uniform->second->getType(), (int)uniform->second->getSize());
		return;
	}

	glUniformMatrix4fv(position, uniform->second->getSize(), GL_FALSE, data->getData());

	CHECK_GL_ERROR;
}

void GLProgram::setUniformTexture(GLuint position, int activeBlock)
{
	auto uniform = m_uniforms.find(position);
	if (uniform == m_uniforms.end())
	{
		LOGE("Not uniform found for position: %u", position);
		return;
	}

	if (uniform->second->getType() != GL_SAMPLER_2D or uniform->second->getSize() != 1)
	{
		LOGE("Incorrect type or size uniform, want Sampler2D, have: %d, size: %d", (int)uniform->second->getType(), (int)uniform->second->getSize());
		return;
	}

	glUniform1i(position, activeBlock);

	CHECK_GL_ERROR;
}

void GLProgram::setUniformVec2(GLuint position, const Vec2& data)
{
	auto uniform = m_uniforms.find(position);
	if (uniform == m_uniforms.end())
	{
		LOGE("Not uniform found for position: %u", position);
		return;
	}

	if (uniform->second->getType() != GL_FLOAT_VEC2 or uniform->second->getSize() != 1)
	{
		LOGE("Incorrect type or size uniform, want Vec2, have: %d, size: %d", (int)uniform->second->getType(), (int)uniform->second->getSize());
		return;
	}

	glUniform2f(position, data.x, data.y);

	CHECK_GL_ERROR;
}

void GLProgram::setUniformFloat(GLuint position, float data)
{
	auto uniform = m_uniforms.find(position);
	if (uniform == m_uniforms.end())
	{
		LOGE("Not uniform found for position: %u", position);
		return;
	}

	if (uniform->second->getType() != GL_FLOAT or uniform->second->getSize() != 1)
	{
		LOGE("Incorrect type or size uniform, want float, have: %d, size: %d", (int)uniform->second->getType(), (int)uniform->second->getSize());
		return;
	}

	glUniform1f(position, data);

	CHECK_GL_ERROR;
}