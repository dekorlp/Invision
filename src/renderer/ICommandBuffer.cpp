#include "precompiled.h"

#include "ICommandBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	ICommandBuffer::ICommandBuffer(IGraphicsInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		instance(instance)
	{
	}

	ICommandBuffer::ICommandBuffer(IGraphicsInstance* instance, unsigned int countOfBuffers) :
		instance(instance)
	{
	}
}