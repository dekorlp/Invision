#ifndef VULKAN_ENGINE_H
#define VULKAN_ENGINE_H

#include <memory>
#include "IGraphicsEngine.h"
#include "renderer\Vulkan\VulkanBase.h"
#include "renderer\Vulkan\VulkanBaseException.h"
#include "renderer\Vulkan\VulkanBaseInstance.h"
#include "renderer\Vulkan\VulkanBaseDevice.h"
#include "renderer\Vulkan\VulkanBasePresentation.h"
#include "renderer\Vulkan\VulkanBaseCommandPool.h"
#include "renderer\Vulkan\VulkanBaseMemoryManager.h"
#include "Vulkan/Vulkan.h"

namespace Invision
{
	namespace VulkanDebug
	{
		static std::ofstream* g_ofstr = nullptr;

		static void debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
		{
			if (g_ofstr == nullptr) g_ofstr = static_cast<std::ofstream*>(pUserData);
			*g_ofstr << "validation layer: " << pCallbackData->pMessage << std::endl;
		}
	}

	class VulkanEngine : public IGraphicsEngine
	{
	private:
		Invision::VulkanBaseInstance vulkanInstance;
		Invision::SVulkanBase vulkInstance;
		Invision::VulkanBaseCommandPool commandPool;
		Invision::VulkanBaseMemoryManager mMemoryManager;

		PhysicalDeviceProperties ConvertPhysicalDeviceParameters(SVulkanBasePhysicalDevice physicalDeviceStruct);

	public:
		INVISION_API VulkanEngine();
		INVISION_API VulkanEngine(std::ofstream* ofstr);

		INVISION_API void Init(MSAAMode msaa = MSAAMODE_OFF);
		INVISION_API void Init(unsigned int index, MSAAMode msaa = MSAAMODE_OFF);
		
		INVISION_API Invision::SVulkanBase& GetVulkanInstance();
		INVISION_API Invision::VulkanBaseCommandPool GetCommandPool();
		
		INVISION_API std::shared_ptr<IGraphicsInstance> CreateInstance(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest) override;

		INVISION_API std::vector< PhysicalDeviceProperties> GetPhysicalDevices();
		INVISION_API Invision::VulkanBaseMemoryManager& GetMemoryManager();

		INVISION_API ~VulkanEngine();

	};

}
#endif // VULKAN_ENGINE_H