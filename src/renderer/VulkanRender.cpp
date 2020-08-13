#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanRender.h"
#include "VulkanCommandBuffer.h"
namespace Invision
{

	/*VulkanRenderer::VulkanRenderer(VulkanEngine* engine) :
		IRenderer(engine)
	{
	}*/

	VulkanRenderer::VulkanRenderer(VulkanInstance* instance)
		: IRenderer(instance)
	{
		vulkanInstance = instance;
		renderer.CreateSyncObjects(instance->GetCoreEngine()->GetVulkanInstance(), instance->GetVulkanContext());
	}

	bool VulkanRenderer::PrepareFrame()
	{
		bool recreateSwapchainIsNecessary = false;

		//VkResult nextImageResult = commandBuffer.AquireNextImage(vulkInstance);
		VkResult nextImageResult = renderer.AquireNextImage(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext());
		if (nextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchainIsNecessary = true;
			//RecreateSwapChain(m_Size.GetWidth(), m_Size.GetHeight());
			return 0;
		}
		else if (nextImageResult != VK_SUCCESS) {
			throw Invision::VulkanBaseException("failed to acquire swap chain image!");
		}

		return recreateSwapchainIsNecessary;
	}

	bool VulkanRenderer::SubmitFrame(std::shared_ptr<ICommandBuffer> commandBuffer)
	{
		bool recreateSwapchainIsNecessary = false;

		//VkResult drawFrameResult = commandBuffer.DrawFrame(vulkInstance);
		VkResult drawFrameResult = renderer.DrawFrame(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer());
		if (drawFrameResult == VK_ERROR_OUT_OF_DATE_KHR || drawFrameResult == VK_SUBOPTIMAL_KHR) {
			recreateSwapchainIsNecessary = true;
			//RecreateSwapChain(m_Size.GetWidth(), m_Size.GetHeight());
		}
		else if (drawFrameResult != VK_SUCCESS) {
			throw Invision::VulkanBaseException("failed to present swap chain image!");
		}

		return recreateSwapchainIsNecessary;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		renderer.DestroySemaphores(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
}