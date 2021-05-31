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
		mRenderer.CreateSyncObjects(instance->GetCoreEngine()->GetVulkanBaseStruct(), instance->GetVulkanContext());
	}

	bool VulkanRenderer::PrepareFrame()
	{
		bool recreateSwapchainIsNecessary = false;

		//VkResult nextImageResult = commandBuffer.AquireNextImage(vulkInstance);
		VkResult nextImageResult = mRenderer.AquireNextImage(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mImageIndex);
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
			if (mSelectedSemaphore != VK_NULL_HANDLE)
			{
				mRenderer.AlterSubmitInfoWaitSemaphore(1, &mSelectedSemaphore);
			}

			mRenderer.AlterSubmitInfoSignalSemaphore(1, mRenderer.GetSemaphoresRenderComplete());
			// main Command Buffer
			mRenderer.AlterSubmitInfo(1, dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(mImageIndex).GetCommandBuffer());
			mRenderer.DrawFrame(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(mImageIndex));
		}
		else
		{
			mSelectedSemaphore = dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(0).GetSemaphore();

			mRenderer.AlterSubmitInfoWaitSemaphore(1, mRenderer.GetSemaphoresPresentComplete());
			mRenderer.AlterSubmitInfoSignalSemaphore(1, &mSelectedSemaphore);

			// secondary Command Buffer for offscreen rendering
			mRenderer.AlterSubmitInfo(1, dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(0).GetCommandBuffer());
			mRenderer.DrawFrame(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer(0));	
		}

		
	}

	bool VulkanRenderer::SubmitFrame()
	{
		bool recreateSwapchainIsNecessary = false;

		VkResult submitFrameResult = mRenderer.QueuePresent(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mImageIndex);
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
		mRenderer.DestroySemaphores(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}
}