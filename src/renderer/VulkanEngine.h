#ifndef VULKAN_ENGINE_H
#define VULKAN_ENGINE_H

#include "IGraphicsEngine.h"
#include "renderer\Vulkan\InVulkan.h"
#include "renderer\Vulkan\VulkanException.h"
#include "renderer\Vulkan\VulkanInstance.h"
#include "renderer\Vulkan\VulkanDevice.h"

#include "Vulkan/Vulkan.h"

namespace Invision
{
	

	static void debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
	{
		std::stringstream ss;
		ss << "validation layer: " << pCallbackData->pMessage;
		INVISION_LOG_RAWTEXT(ss.str());
	}

	class VulkanEngine : public IGraphicsEngine
	{
	private:
		Invision::VulkanInstance vulkanInstance;
		Invision::SVulkan vulkInstance;

	public:
		INVISION_API VulkanEngine();

		INVISION_API ~VulkanEngine();

	};

}
#endif //VulkanEngine