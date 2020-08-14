#include "precompiled.h"

#include "IVertexBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IVertexBuffer::IVertexBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}
}