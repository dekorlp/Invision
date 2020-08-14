#include "precompiled.h"

#include "IUniformBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IUniformBuffer::IUniformBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}
}