#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseFramebuffer.h"
#include "VulkanBasePipeline.h"
#include "VulkanBaseRenderPass.h"
#include "VulkanBaseCommandBuffer.h"

#include "VulkanBaseRenderer.h"

namespace Invision
{
	void VulkanBaseRenderer::CreateSyncObjects(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext)
	{
		// Fence Info Struct
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		// Semaphore Info Struct
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		// Create Fence and Semaphores
		if (vkCreateSemaphore(vulkanInstance.logicalDevice, &semaphoreInfo, nullptr, &mSemaphores.presentComplete) != VK_SUCCESS ||
			vkCreateSemaphore(vulkanInstance.logicalDevice, &semaphoreInfo, nullptr, &mSemaphores.renderComplete) != VK_SUCCESS ||
			vkCreateFence(vulkanInstance.logicalDevice, &fenceInfo, nullptr, &mRenderFence) != VK_SUCCESS) {

			throw VulkanBaseException("failed to create synchronization objects for a frame!");
		}

		// Create SubmitInfo
		mSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		mSubmitInfo.pWaitDstStageMask = &mWaitStages;
		mSubmitInfo.waitSemaphoreCount = 1;
		mSubmitInfo.pWaitSemaphores = &mSemaphores.presentComplete;
		mSubmitInfo.signalSemaphoreCount = 1;
		mSubmitInfo.pSignalSemaphores = &mSemaphores.renderComplete;
	}

	void VulkanBaseRenderer::AlterSubmitInfo(const uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, const VkPipelineStageFlags* pWaitDstStageMask, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers, uint32_t signalSemaphoreCount, const VkSemaphore* pSignalSemaphores)
	{
		mSubmitInfo.waitSemaphoreCount = waitSemaphoreCount;
		mSubmitInfo.pWaitSemaphores = pWaitSemaphores;
		mSubmitInfo.pWaitDstStageMask = pWaitDstStageMask;
		mSubmitInfo.commandBufferCount = commandBufferCount;
		mSubmitInfo.pCommandBuffers = pCommandBuffers;
		mSubmitInfo.signalSemaphoreCount = signalSemaphoreCount;
		mSubmitInfo.pSignalSemaphores = pSignalSemaphores;
	}

	void VulkanBaseRenderer::AlterSubmitInfo(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
	{
		mSubmitInfo.commandBufferCount = commandBufferCount;
		mSubmitInfo.pCommandBuffers = pCommandBuffers;
	}

	void VulkanBaseRenderer::AlterSubmitInfoSignalSemaphore(uint32_t signalSemaphoreCount, const VkSemaphore* pSignalSemaphores)
	{
		mSubmitInfo.signalSemaphoreCount = signalSemaphoreCount;
		mSubmitInfo.pSignalSemaphores = pSignalSemaphores;
	}

	void VulkanBaseRenderer::AlterSubmitInfoWaitSemaphore(uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores)
	{
		mSubmitInfo.waitSemaphoreCount = waitSemaphoreCount;
		mSubmitInfo.pWaitSemaphores = pWaitSemaphores;
	}

	VkResult VulkanBaseRenderer::AquireNextImage(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int& imageIndex)
	{

		/*VkResult fenceRes;
		do {
			fenceRes = vkWaitForFences(vulkanInstance.logicalDevice, 1, &mRenderFence, VK_TRUE, 100000000);
		} while (fenceRes == VK_TIMEOUT);

		if (fenceRes) {
			throw VulkanBaseException("failed to submit draw command buffer!");
		}

		vkResetFences(vulkanInstance.logicalDevice, 1, &mRenderFence);*/

		VkResult result = vkAcquireNextImageKHR(vulkanInstance.logicalDevice, vulkanContext.swapChain, UINT64_MAX, mSemaphores.presentComplete, VK_NULL_HANDLE, &imageIndex);

		return result;
	}

	void VulkanBaseRenderer::DrawFrame(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandBuffer& commandBuffer)
	{
		if (vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &mSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw VulkanBaseException("failed to submit draw command buffer!");
		}
	}

	VkResult VulkanBaseRenderer::QueuePresent(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int imageIndex)
	{
		VkSwapchainKHR swapChains[] = { vulkanContext.swapChain };

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		// Check if a wait semaphore has been specified to wait for before presenting the image
		if (mSemaphores.renderComplete != VK_NULL_HANDLE)
		{
			presentInfo.pWaitSemaphores = &mSemaphores.renderComplete;
			presentInfo.waitSemaphoreCount = 1;
		}
		VkResult result = vkQueuePresentKHR(vulkanContext.presentQueue, &presentInfo);

		vkQueueWaitIdle(vulkanContext.presentQueue);

		return result;
	}

	VkSemaphore* VulkanBaseRenderer::GetSemaphoresPresentComplete()
	{
		return &mSemaphores.presentComplete;
	}

	VkSemaphore* VulkanBaseRenderer::GetSemaphoresRenderComplete()
	{
		return &mSemaphores.renderComplete;
	}

	void VulkanBaseRenderer::DestroySemaphores(SVulkanBase &vulkanInstance)
	{
		vkDestroySemaphore(vulkanInstance.logicalDevice, mSemaphores.presentComplete, nullptr);
		vkDestroySemaphore(vulkanInstance.logicalDevice, mSemaphores.renderComplete, nullptr);
		vkDestroyFence(vulkanInstance.logicalDevice, mRenderFence, nullptr);
	}
}