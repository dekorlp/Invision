#include "precompiled.h"

#include "IGraphicsInstance.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}
	
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine, CanvasDimensions* canvas, bool activateDepthTest) :
		Engine(engine)
	{
		
	}

	IGraphicsEngine* IGraphicsInstance::engine()
	{
		return Engine;
	}

}