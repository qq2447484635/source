#pragma once
#include "Framework.h"
#include <d2d1.h>
#include "Box2D/Box2D.h"

class _RenderData
{
public:
	_RenderData(b2PolygonShape *s) { shape = s; };
	b2PolygonShape *shape=NULL;
	ComPtr<ID2D1TransformedGeometry> d2dGeometry;
};
