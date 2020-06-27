#include "precompiled.h"

#include "GraphicsFactory.h"

namespace Invision
{
	std::shared_ptr<IGraphicsEngine> create_engine(EngineType::Type type, CanvasDimensions canvas)
	{
		if (type == EngineType::DirectX) return std::make_shared<Invision::VulkanEngine>(canvas);
		//else if (type == EngineType::Vulkan) return std::make_shared<VulkanEngine>();
		return std::shared_ptr<IGraphicsEngine>();
	}

}