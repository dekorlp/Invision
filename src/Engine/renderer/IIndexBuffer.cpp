#include "precompiled.h"

#include "IIndexBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IIndexBuffer::IIndexBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

}