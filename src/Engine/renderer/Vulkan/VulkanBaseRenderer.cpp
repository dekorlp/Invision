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
			vkCreateFence(vulkanInstance.logicalDevice, &fenceInfo, nullptr, &renderFence) != VK_SUCCESS) {

			throw VulkanBaseException("failed to create synchronization objects for a frame!");
		}

		// Create SubmitInfo
		mSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		mSubmitInfo.pWaitDstStageMask = &waitStages;
		mSubmitInfo.waitSemaphoreCount = 1;
		mSubmitInfo.pWaitSemaphores = &mSemaphores.presentComplete;
		mSubmitInfo.signalSemaphoreCount = 1;
		mSubmitInfo.pSignalSemaphores = &mSemaphores.renderComplete;
	}

	VkResult VulkanBaseRenderer::AquireNextImage(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext)
	{

		VkResult fenceRes;
		do {
			fenceRes = vkWaitForFences(vulkanInstance.logicalDevice, 1, &renderFence, VK_TRUE, 100000000);
		} while (fenceRes == VK_TIMEOUT);

		if (fenceRes) {
			throw VulkanBaseException("failed to submit draw command buffer!");
		}

		vkResetFences(vulkanInstance.logicalDevice, 1, &renderFence);

		VkResult result = vkAcquireNextImageKHR(vulkanInstance.logicalDevice, vulkanContext.swapChain, UINT64_MAX, mSemaphores.presentComplete, VK_NULL_HANDLE, &mImageIndex);

		return result;
	}

	void VulkanBaseRenderer::DrawFrame(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandBuffer& commandBuffer)
	{
		if(commandBuffer.GetCommandBuffers().size() < vulkanContext.swapChainImages.size())
			throw VulkanBaseException("failed to submit draw command buffer! Create more CommandBuffers to DrawOnScreen");

		mSubmitInfo.commandBufferCount = 1;
		
		mSubmitInfo.pCommandBuffers = commandBuffer.GetCommandBuffer(mImageIndex);

		if (vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &mSubmitInfo, renderFence) != VK_SUCCESS) {
			throw VulkanBaseException("failed to submit draw command buffer!");
		}
	}

	void VulkanBaseRenderer::DrawFrameOffscreen(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandBuffer& commandBuffer)
	{
		mSubmitInfo.commandBufferCount = 1;

		mSubmitInfo.pCommandBuffers = commandBuffer.GetCommandBuffer(0); // Offscreen has only one commandBuffer

		if (vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &mSubmitInfo, renderFence) != VK_SUCCESS) {
			throw VulkanBaseException("failed to submit draw command buffer!");
		}
	}

	VkResult VulkanBaseRenderer::QueuePresent(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext)
	{
		VkSwapchainKHR swapChains[] = { vulkanContext.swapChain };

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &mImageIndex;
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

	void VulkanBaseRenderer::DestroySemaphores(SVulkanBase &vulkanInstance)
	{
		vkDestroySemaphore(vulkanInstance.logicalDevice, mSemaphores.presentComplete, nullptr);
		vkDestroySemaphore(vulkanInstance.logicalDevice, mSemaphores.renderComplete, nullptr);
		vkDestroyFence(vulkanInstance.logicalDevice, renderFence, nullptr);
	}
}