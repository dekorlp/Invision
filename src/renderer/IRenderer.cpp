#include "precompiled.h"

#include "IRenderer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IRenderer::IRenderer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

}