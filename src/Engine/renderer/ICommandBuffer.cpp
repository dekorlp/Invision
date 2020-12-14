#include "precompiled.h"

#include "ICommandBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	ICommandBuffer::ICommandBuffer(IGraphicsInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		instance(instance)
	{
	}
}