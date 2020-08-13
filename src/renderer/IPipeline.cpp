#include "precompiled.h"

#include "IPipeline.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"

namespace Invision
{
	IPipeline::IPipeline(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IGraphicsEngine* IPipeline::engine()
	{
		return Engine;
	}

}