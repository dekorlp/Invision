#include "precompiled.h"

#include "IPushConstant.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IPushConstant::IPushConstant(IGraphicsInstance* instance) :
		instance(instance)
	{
	}

	IPushConstant::IPushConstant(IGraphicsInstance* instance, ShaderStage shaderStages, uint32_t offset, uint32_t size) :
		instance(instance)
	{

	}

}