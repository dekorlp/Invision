#include "precompiled.h"

#include "IVertexBuffer.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IVertexBuffer::IVertexBuffer(IGraphicsInstance* instance) :
		mInstance(instance)
	{
	}

	IBindingDescription::IBindingDescription(IGraphicsInstance* instance) : mInstance(instance)
	{

	}

	
	IAttributeDescription::IAttributeDescription(IGraphicsInstance* instance) :
		mInstance(instance)
	{
	}
	
}