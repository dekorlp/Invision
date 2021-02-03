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
		mVulkanInstance = instance;
		mRenderer.CreateSyncObjects(instance->GetCoreEngine()->GetVulkanInstance(), instance->GetVulkanContext());
	}

	bool VulkanRenderer::PrepareFrame()
	{
		bool recreateSwapchainIsNecessary = false;

		//VkResult nextImageResult = commandBuffer.AquireNextImage(vulkInstance);
		VkResult nextImageResult = mRenderer.AquireNextImage(mVulkanInstance->GetCoreEngine()->GetVulkanInstance(), mVulkanInstance->GetVulkanContext(), mImageIndex);
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

	void VulkanRenderer::Draw(std::shared_ptr<ICommandBuffer> commandBuffer)
	{
		// check if it is main CommandBuffer or not
		if (dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCountOfCommandBuffers() > 1)
		{
			// main Command Buffer
			mRenderer.DrawFrame(mVulkanInstance->GetCoreEngine()->GetVulkanInstance(), mVulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(mImageIndex));
		}
		else
		{
			// secondary Command Buffer for offscreen rendering
			mRenderer.DrawFrame(mVulkanInstance->GetCoreEngine()->GetVulkanInstance(), mVulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(0));
		}

		
	}

	bool VulkanRenderer::SubmitFrame()
	{
		bool recreateSwapchainIsNecessary = false;

		VkResult submitFrameResult = mRenderer.QueuePresent(mVulkanInstance->GetCoreEngine()->GetVulkanInstance(), mVulkanInstance->GetVulkanContext(), mImageIndex);
		if (submitFrameResult == VK_ERROR_OUT_OF_DATE_KHR || submitFrameResult == VK_SUBOPTIMAL_KHR) {
			recreateSwapchainIsNecessary = true;
			//RecreateSwapChain(m_Size.GetWidth(), m_Size.GetHeight());
		}
		else if (submitFrameResult != VK_SUCCESS) {
			throw Invision::VulkanBaseException("failed to present swap chain image!");
		}

		return recreateSwapchainIsNecessary;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		mRenderer.DestroySemaphores(mVulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
}