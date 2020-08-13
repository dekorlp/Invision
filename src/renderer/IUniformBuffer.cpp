#include "precompiled.h"

#include "IUniformBuffer.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	IUniformBuffer::IUniformBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IGraphicsEngine* IUniformBuffer::engine()
	{
		return Engine;
	}

}