#include "precompiled.h"

#include "IRenderPass.h"

#include "IGraphicsEngine.h"
#include "IGraphicsInstance.h"


namespace Invision
{
	/*IRenderPass::IRenderPass(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}*/

	IRenderPass::IRenderPass(IGraphicsInstance* instance) :
		instance(instance)
	{
		
	}

	IGraphicsEngine* IRenderPass::engine()
	{
		return Engine;
	}

}