#include "precompiled.h"

#include "ICommandBuffer.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	ICommandBuffer::ICommandBuffer(IGraphicsEngine* engine, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		Engine(engine)
	{
	}

	IGraphicsEngine* ICommandBuffer::engine()
	{
		return Engine;
	}

}