#include "precompiled.h"

#include "IRenderer.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	IRenderer::IRenderer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IGraphicsEngine* IRenderer::engine()
	{
		return Engine;
	}

}