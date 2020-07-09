#include "precompiled.h"

#include "IPipeline.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IPipeline::IPipeline(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}

	IGraphicsEngine* IPipeline::engine()
	{
		return Engine;
	}

}