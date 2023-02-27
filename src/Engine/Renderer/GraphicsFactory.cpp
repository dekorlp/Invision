#include "precompiled.h"

#include "GraphicsFactory.h"


namespace Invision
{
	std::shared_ptr<IGraphicsEngine> Create_engine(EngineType::Type type)
	{
		if (type == EngineType::Vulkan) return std::make_shared<Invision::VulkanEngine>();
		//else if (type == EngineType::Vulkan) return std::make_shared<VulkanEngine>();
		return std::shared_ptr<IGraphicsEngine>();
	}

	std::shared_ptr<IGraphicsEngine> Create_engine(EngineType::Type type, std::ofstream* ofstr)
	{
		if (type == EngineType::Vulkan) return std::make_shared<Invision::VulkanEngine>(ofstr);
		//else if (type == EngineType::Vulkan) return std::make_shared<VulkanEngine>();
		return std::shared_ptr<IGraphicsEngine>();
	}

}