#include "precompiled.h"

#include "IFramebuffer.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IFramebuffer::IFramebuffer(IGraphicsEngine* engine, std::shared_ptr<IRenderPass> renderPass) :
		Engine(engine)
	{
		int test = 0;
	}

	IGraphicsEngine* IFramebuffer::engine()
	{
		return Engine;
	}

}