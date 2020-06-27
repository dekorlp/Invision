#include "precompiled.h"

#include "VulkanEngine.h"


namespace Invision
{
	VulkanEngine::VulkanEngine() :
		IGraphicsEngine(EngineType::Vulkan, "Vulkan", "1.2.137")
	{
		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
		vulkanInstance.SetDebugMessanger(debugCallback);
	}

	VulkanEngine::~VulkanEngine()
	{
		vulkanInstance.Destroy();
	}

}