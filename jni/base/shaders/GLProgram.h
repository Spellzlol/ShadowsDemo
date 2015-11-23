#pragma once

#include <GLES2/gl2.h>
#include <map>
#include <memory>
#include "Geometry.h"

//----------------------------------------------------------------------------------------------------------------
// GLProgramAttribute
//----------------------------------------------------------------------------------------------------------------

class GLProgramAttribute
{
public:
	GLProgramAttribute(const std::string& name, GLuint position);

	const std::string& getName() const;
	GLuint getPosition() const;

private:
	std::string m_name;
	GLuint m_position = 0;
};

//----------------------------------------------------------------------------------------------------------------
// GLProgramUniform
//----------------------------------------------------------------------------------------------------------------

class GLProgramUniform
{
public:
	GLProgramUniform(const std::string& name, GLuint position, GLint size, GLenum type);

	const std::string& getName() const;
	GLuint getPosition() const;
	GLenum getType() const;
	GLint getSize() const;

private:
	std::string m_name;
	GLuint m_position = 0;
	GLint m_size = 0;
	GLenum m_type = GL_FLOAT;
};

//----------------------------------------------------------------------------------------------------------------
// GLProgram
//----------------------------------------------------------------------------------------------------------------

class Texture;
class GLProgram
{
public:
	static GLProgram* create(const char* vShader, const char* fShader);
	~GLProgram();

public:
	void addAttribute(GLProgramAttribute *attribute);
	void linkProgram();
	void use();
	void recover();

	GLint findUniformForName(const std::string& name);

	void setUniformVec4(GLuint position, const Vec4& data);
	void setUniformVec3(GLuint position, const Vec3& data);
	void setUniformVec2(GLuint position, const Vec2& data);
	void setUniformMat4(GLuint position, const Mat4* data);
	void setUniformTexture(GLuint position, int activeBlock);
	void setUniformFloat(GLuint position, float var);

private:
	GLProgram() = default;

	static GLuint loadAndCompileShader(const char* source, GLenum type);
	void parseUniforms();

private:
	GLuint m_programObject = 0;
	GLuint m_vertexShader = 0;
	GLuint m_fragmentShader = 0;

	const char* m_vShader = nullptr;
	const char* m_fShader = nullptr;

	std::map<GLuint, std::unique_ptr<GLProgramAttribute>> m_attributes;
	std::map<GLint, std::unique_ptr<GLProgramUniform>> m_uniforms;
};