#include "precompiled.h"

#include "IFramebuffer.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	IFramebuffer::IFramebuffer(IGraphicsInstance* instance, std::shared_ptr<IRenderPass> renderPass) :
		instance(instance)
	{
	}

	IGraphicsEngine* IFramebuffer::engine()
	{
		return Engine;
	}

}