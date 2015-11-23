#pragma once

#include "PrimitiveNode.h"

class RectangleNode : public PrimitiveNode
{
public:
	static RectangleNode* create(const Vec3& position, const Size& size, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

public:
	virtual void updateTransform() override;
	virtual void update(float dt) override;

protected:
	RectangleNode(PositionColor* vertexes, size_t vertexCount);
};