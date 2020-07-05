#include "precompiled.h"

#include "IUniformBuffer.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IUniformBuffer::IUniformBuffer(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}

	IGraphicsEngine* IUniformBuffer::engine()
	{
		return Engine;
	}

}