#include "precompiled.h"

#include "VulkanRender.h"
#include "VulkanRenderPass.h"
#include "VulkanVertexBuffer.h"
#include "VulkanUniformBuffer.h"

#include "VulkanEngine.h"



namespace Invision
{


	VulkanEngine::VulkanEngine(CanvasDimensions canvas) :
		IGraphicsEngine(EngineType::Vulkan, "Vulkan", "1.2.137", canvas)
	{
		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
		vulkanInstance.SetDebugMessanger(VulkanDebug::debugCallback);
		Invision::CreateSurface(vulkInstance, canvas.hwnd);
		Invision::CreateVulkanDevice(vulkInstance);
		Invision::CreatePresentationSystem(vulkInstance, canvas.width, canvas.height);
		commandPool.CreateCommandPool(vulkInstance);
	}

	std::shared_ptr <IRenderer> VulkanEngine::create_renderer()
	{
		return  std::make_shared<VulkanRenderer >(this);
	}

	std::shared_ptr<IRenderPass> VulkanEngine::CreateRenderPass()
	{
		return  std::make_shared<VulkanRenderPass>(this);
	}

	std::shared_ptr<IVertexBuffer> VulkanEngine::CreateVertexBuffer()
	{
		return  std::make_shared<VulkanVertexBuffer>(this);
	}

	std::shared_ptr<IUniformBuffer> VulkanEngine::CreateUniformBuffer()
	{
		return std::make_shared<VulkanUniformBuffer>(this);
	}

	Invision::SVulkanBase VulkanEngine::GetVulkanInstance()
	{
		return vulkInstance;
	}

	Invision::VulkanBaseCommandPool VulkanEngine::GetCommandPool()
	{
		return commandPool;
	}

	VulkanEngine::~VulkanEngine()
	{
		commandPool.DestroyCommandPool(vulkInstance);
		Invision::DestroyPresentationSystem(vulkInstance);
		Invision::DestroyVulkanDevice(vulkInstance);
		Invision::DestroySurface(vulkInstance);
		vulkanInstance.Destroy();
	}

}