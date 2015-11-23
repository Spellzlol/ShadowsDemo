#pragma once

#include "Node.h"
#include <GLES2/gl2.h>

class PrimitiveNode : public Node
{
protected:
	PrimitiveNode(PositionColor* vertexes, size_t vertexCount, GLubyte* indices = nullptr, size_t indicesCount = 0);
	virtual ~PrimitiveNode();

public:
	virtual void draw() override;
	virtual void updateColor() override;
	virtual void updateRotation() override;

	void useIndices(bool use);

protected:
	GLuint m_vbo = -1;
	GLuint m_ibo = -1;
	GLuint m_aPosition = 0;
	GLuint m_aColor = 1;

	PositionColor* m_vertexes = 0;
	size_t m_vertexCount = 0;
	size_t m_indicesCount = 0;

	bool m_useindices = false;
};