#include "precompiled.h"

#include "IGraphicsInstance.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine) :
		mEngine(engine)
	{
	}
	
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine, CanvasDimensions* canvas, MSAAMode msaaMode) :
		mEngine(engine)
	{
		
	}

	IGraphicsEngine* IGraphicsInstance::GetEngine()
	{
		return mEngine;
	}

}