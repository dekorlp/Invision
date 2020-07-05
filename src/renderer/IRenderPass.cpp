#include "precompiled.h"

#include "IRenderPass.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IRenderPass::IRenderPass(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}

	IGraphicsEngine* IRenderPass::engine()
	{
		return Engine;
	}

}