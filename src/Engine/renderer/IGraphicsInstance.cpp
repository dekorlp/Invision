#include "precompiled.h"

#include "IGraphicsInstance.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}
	
	IGraphicsInstance::IGraphicsInstance(IGraphicsEngine* engine, CanvasDimensions* canvas, bool activateDepthTest, MSAAMode msaa) :
		Engine(engine)
	{
		
	}

	IGraphicsEngine* IGraphicsInstance::engine()
	{
		return Engine;
	}

}