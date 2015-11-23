#pragma once

#include "PrimitiveNode.h"

class LightRectangleNode : public PrimitiveNode
{
public:
	static LightRectangleNode* create(const Vec3& position, const Size& size, const Color4& color = Color4(1.0f,1.0f,1.0f,1.0f));

public:
	virtual void updateTransform() override;
	virtual void update(float dt) override;
	virtual void draw() override;

protected:
	LightRectangleNode(PositionColor* vertexes, size_t vertexCount);
};