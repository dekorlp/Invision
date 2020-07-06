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
#include "Vulkan/Vulkan.h"

namespace Invision
{
	namespace VulkanDebug
	{
		static void debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
		{
			std::stringstream ss;
			ss << "validation layer: " << pCallbackData->pMessage;
			INVISION_LOG_RAWTEXT(ss.str());
		}
	}

	class VulkanEngine : public IGraphicsEngine
	{
	private:
		Invision::VulkanBaseInstance vulkanInstance;
		Invision::SVulkanBase vulkInstance;
		Invision::VulkanBaseCommandPool commandPool;

		PhysicalDeviceProperties ConvertPhysicalDeviceParameters(SVulkanBasePhysicalDevice physicalDeviceStruct);

	public:
		INVISION_API VulkanEngine();

		INVISION_API void Init(CanvasDimensions canvas);
		INVISION_API void Init(unsigned int index, CanvasDimensions canvas);

		INVISION_API Invision::SVulkanBase GetVulkanInstance();
		INVISION_API Invision::VulkanBaseCommandPool GetCommandPool();
		INVISION_API std::shared_ptr <IRenderer> create_renderer() override;
		INVISION_API std::shared_ptr <IRenderPass> CreateRenderPass() override; 
		INVISION_API std::shared_ptr<IVertexBuffer> CreateVertexBuffer() override;
		INVISION_API std::shared_ptr<IUniformBuffer> CreateUniformBuffer() override;
		INVISION_API std::shared_ptr<IIndexBuffer> CreateIndexBuffer() override;


		INVISION_API std::vector< PhysicalDeviceProperties> GetPhysicalDevices();

		INVISION_API ~VulkanEngine();

	};

}
#endif // VULKAN_ENGINE_H