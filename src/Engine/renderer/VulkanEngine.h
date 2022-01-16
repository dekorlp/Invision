#ifndef VULKAN_ENGINE_H
#define VULKAN_ENGINE_H

#include <memory>
#include "IGraphicsEngine.h"
#include "renderer\Vulkan\VulkanBase.h"
#include "renderer\Vulkan\VulkanBaseException.h"
#include "renderer\Vulkan\VulkanBaseInstance.h"
#include "renderer\Vulkan\VulkanBaseDevice.h"
#include "renderer\Vulkan\VulkanBasePresentation.h"
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
		Invision::VulkanBaseInstance mVulkanInstance;
		Invision::SVulkanBase mBaseStruct;

		PhysicalDeviceProperties ConvertPhysicalDeviceParameters(SVulkanBasePhysicalDevice physicalDeviceStruct);

	public:
		INVISION_API VulkanEngine();
		INVISION_API VulkanEngine(std::ofstream* ofstr);

		INVISION_API void Init();
		INVISION_API void Init(unsigned int index);
		
		INVISION_API Invision::SVulkanBase& GetVulkanBaseStruct();
		
		
		INVISION_API std::shared_ptr<IGraphicsInstance> CreateInstance(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, MSAAMode msaaMode) override;

		INVISION_API std::vector< PhysicalDeviceProperties> GetPhysicalDevices();

	

		INVISION_API ~VulkanEngine();

	};

}
#endif // VULKAN_ENGINE_H