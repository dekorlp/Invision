#include "precompiled.h"

#include "IFramebuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IFramebuffer::IFramebuffer(IGraphicsInstance* instance, std::shared_ptr<IRenderPass> renderPass) :
		mInstance(instance)
	{
	}


}