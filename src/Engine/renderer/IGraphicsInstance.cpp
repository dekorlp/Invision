#include "precompiled.h"

#include "IGraphicsInstance.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine) :
		mEngine(engine)
	{
	}
	
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine, CanvasDimensions* canvas, bool activateDepthTest) :
		mEngine(engine)
	{
		
	}

	IGraphicsEngine* IGraphicsInstance::engine()
	{
		return mEngine;
	}

}