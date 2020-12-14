#include "precompiled.h"

#include "IVertexBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IVertexBuffer::IVertexBuffer(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IBindingDescription::IBindingDescription(IGraphicsInstance* instance) : instance(instance)
	{

	}

	
	IAttributeDescription::IAttributeDescription(IGraphicsInstance* instance) :
		instance(instance)
	{
	}
	
}