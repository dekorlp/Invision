#include "precompiled.h"

#include "IIndexBuffer.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	IIndexBuffer::IIndexBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IGraphicsEngine* IIndexBuffer::engine()
	{
		return Engine;
	}

}