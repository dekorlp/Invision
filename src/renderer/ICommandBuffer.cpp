#include "precompiled.h"

#include "ICommandBuffer.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	ICommandBuffer::ICommandBuffer(IGraphicsInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		instance(instance)
	{
	}

	IGraphicsEngine* ICommandBuffer::engine()
	{
		return Engine;
	}

}