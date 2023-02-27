#include "precompiled.h"

#include "IPipeline.h"

#include "IGraphicsInstance.h"


namespace Invision
{
	IPipeline::IPipeline(IGraphicsInstance* instance) :
		mInstance(instance)
	{
	}

	IPipeline::IPipeline(IGraphicsInstance* instance, PipelineProperties* pipelineProperties) :
		mInstance(instance)
	{
	}

}