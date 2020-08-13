#include "precompiled.h"

#include "IVertexBuffer.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	IVertexBuffer::IVertexBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IGraphicsEngine* IVertexBuffer::engine()
	{
		return Engine;
	}

}