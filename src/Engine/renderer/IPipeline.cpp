#include "precompiled.h"

#include "IPipeline.h"

#include "IGraphicsInstance.h"


namespace Invision
{
	IPipeline::IPipeline(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IPipeline::IPipeline(IGraphicsInstance* instance, PipelineProperties* pipelineProperties) :
		instance(instance)
	{
	}

}