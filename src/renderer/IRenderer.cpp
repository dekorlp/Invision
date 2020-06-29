#include "precompiled.h"

#include "IRenderer.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IRenderer::IRenderer(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}

	IGraphicsEngine* IRenderer::engine()
	{
		return Engine;
	}

}