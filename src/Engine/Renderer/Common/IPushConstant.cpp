#include "precompiled.h"

#include "IPushConstant.h"

#include "IGraphicsInstance.h"

namespace Invision
{
	IPushConstant::IPushConstant(IGraphicsInstance* instance) :
		mInstance(instance)
	{
	}

	IPushConstant::IPushConstant(IGraphicsInstance* instance, ShaderStageFlag shaderStages, uint32_t offset, uint32_t size) :
		mInstance(instance)
	{

	}

}