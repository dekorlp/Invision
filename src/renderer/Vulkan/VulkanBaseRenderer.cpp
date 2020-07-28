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
	const int MAX_FRAMES_IN_FLIGHT = 2;

	void VulkanBaseRenderer::CreateSyncObjects(SVulkanBase &vulkanInstance)
	{
		mImageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
		mRenderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
		mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		mImagesInFlight.resize(vulkanInstance.swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(vulkanInstance.logicalDevice, &semaphoreInfo, nullptr, &mImageAvailableSemaphore[i]) != VK_SUCCESS ||
				vkCreateSemaphore(vulkanInstance.logicalDevice, &semaphoreInfo, nullptr, &mRenderFinishedSemaphore[i]) != VK_SUCCESS ||
				vkCreateFence(vulkanInstance.logicalDevice, &fenceInfo, nullptr, &mInFlightFences[i]) != VK_SUCCESS) {

				throw VulkanBaseException("failed to create synchronization objects for a frame!");
			}
		}
	}

	VkResult VulkanBaseRenderer::AquireNextImage(SVulkanBase &vulkanInstance)
	{
		vkWaitForFences(vulkanInstance.logicalDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

		vulkanInstance.mImageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(vulkanInstance.logicalDevice, vulkanInstance.swapChain, UINT64_MAX, mImageAvailableSemaphore[mCurrentFrame], VK_NULL_HANDLE, &vulkanInstance.mImageIndex);
		return result;
	}

	VkResult VulkanBaseRenderer::DrawFrame(SVulkanBase &vulkanInstance, VulkanBaseCommandBuffer& commandBuffer)
	{
		
		if (mImagesInFlight[vulkanInstance.mImageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(vulkanInstance.logicalDevice, 1, &mImagesInFlight[vulkanInstance.mImageIndex], VK_TRUE, UINT64_MAX);
		}

		mImagesInFlight[vulkanInstance.mImageIndex] = mInFlightFences[mCurrentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore[mCurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = commandBuffer.GetCommandBuffer(vulkanInstance.mImageIndex);


		VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphore[mCurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(vulkanInstance.logicalDevice, 1, &mInFlightFences[mCurrentFrame]);

		if (vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &submitInfo, mInFlightFences[mCurrentFrame]) != VK_SUCCESS) {
			throw VulkanBaseException("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { vulkanInstance.swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &vulkanInstance.mImageIndex;
		presentInfo.pResults = nullptr; // Optional


		VkResult result = vkQueuePresentKHR(vulkanInstance.presentQueue, &presentInfo);

		mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		vkQueueWaitIdle(vulkanInstance.presentQueue);

		return result;
	}

	void VulkanBaseRenderer::DestroySemaphores(SVulkanBase &vulkanInstance)
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(vulkanInstance.logicalDevice, mImageAvailableSemaphore[i], nullptr);
			vkDestroySemaphore(vulkanInstance.logicalDevice, mRenderFinishedSemaphore[i], nullptr);
			vkDestroyFence(vulkanInstance.logicalDevice, mInFlightFences[i], nullptr);
		}
		mImageAvailableSemaphore.clear();
		mRenderFinishedSemaphore.clear();
		mInFlightFences.clear();
		mImagesInFlight.clear();
	}
}