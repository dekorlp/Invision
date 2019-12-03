#include "precompiled.h"

#include "VulkanCommandBuffer.h"

namespace Invision
{
	void VulkanCommandBuffer::CreateCommandPool(SVulkan &vulkanInstance)
	{
		SQueueFamilyIndices queueFamilyIndices = Invision::FindQueueFamilies(vulkanInstance.physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0;

		if (vkCreateCommandPool(vulkanInstance.logicalDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
		{
			throw VulkanException("failed to create Command pool!");
		}
	}

	

	void VulkanCommandBuffer::CreateCommandBuffers(SVulkan &vulkanInstance, VulkanFramebuffer &vulkanFramebuffer, VulkanPipeline &vulkanPipeline, VulkanRenderPass &renderPass, VkViewport viewportParam, VkRect2D scissorParam)
	{
		mCommandBuffers.resize(vulkanFramebuffer.GetFramebuffers().size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32)mCommandBuffers.size();

		if (vkAllocateCommandBuffers(vulkanInstance.logicalDevice, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
		{
			throw VulkanException("failed to create command buffers!");
		}

		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				throw VulkanException("failed to begin recording command buffer!");
			}
		

			if (viewportParam.height == 0 && viewportParam.width == 0)
			{
				// there is no viewport used
				VkViewport viewport = {};
				viewport.x = 0.0f; // default: 0.0f;
				viewport.y = 0.0f; // default: 0.0f;
				viewport.width = (float)vulkanInstance.swapChainExtent.width; // default: (float)vulkanInstance.swapchainExtend.Width
				viewport.height = (float)vulkanInstance.swapChainExtent.height;// default: (float)vulkanInstance.swapchainExtend.Height
				viewport.minDepth = 0.0; // default: 0.0
				viewport.maxDepth = 1.0; // default: 1.0
				vkCmdSetViewport(mCommandBuffers[i], 0, 1, &viewport);
			}
			else
			{
				vkCmdSetViewport(mCommandBuffers[i], 0, 1, &viewportParam);
			}

			if (scissorParam.extent.height == 0 && scissorParam.extent.width == 0)
			{
				VkRect2D scissor = {};
				scissor.offset = { 0, 0 }; // default: { 0, 0 };
				scissor.extent = vulkanInstance.swapChainExtent;
				vkCmdSetScissor(mCommandBuffers[i], 0, 1, &scissor);
			}
			else
			{
				vkCmdSetScissor(mCommandBuffers[i], 0, 1, &scissorParam);
			}



			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass.GetRenderPass();
			renderPassInfo.framebuffer = vulkanFramebuffer.GetFramebuffers()[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = vulkanInstance.swapChainExtent;

			VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.GetPipeline());
			vkCmdDraw(mCommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(mCommandBuffers[i]);

			if (vkEndCommandBuffer(mCommandBuffers[i]) != VK_SUCCESS)
			{
				throw VulkanException("failed to record command buffer!");
			}
		}
	}

	void VulkanCommandBuffer::CreateSyncObjects(SVulkan &vulkanInstance)
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

				throw VulkanException("failed to create synchronization objects for a frame!");
			}
		}
	}

	VkResult VulkanCommandBuffer::AquireNextImage(SVulkan &vulkanInstance)
	{
		vkWaitForFences(vulkanInstance.logicalDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

		mImageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(vulkanInstance.logicalDevice, vulkanInstance.swapChain, UINT64_MAX, mImageAvailableSemaphore[mCurrentFrame], VK_NULL_HANDLE, &mImageIndex);
		return result;
	}

	VkResult VulkanCommandBuffer::DrawFrame(SVulkan &vulkanInstance)
	{
		

		if (mImagesInFlight[mImageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(vulkanInstance.logicalDevice, 1, &mImagesInFlight[mImageIndex], VK_TRUE, UINT64_MAX);
		}

		mImagesInFlight[mImageIndex] = mInFlightFences[mCurrentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore[mCurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffers[mImageIndex];

		VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphore[mCurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(vulkanInstance.logicalDevice, 1, &mInFlightFences[mCurrentFrame]);

		if (vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &submitInfo, mInFlightFences[mCurrentFrame]) != VK_SUCCESS) {
			throw VulkanException("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { vulkanInstance.swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &mImageIndex;
		presentInfo.pResults = nullptr; // Optional
		VkResult result = vkQueuePresentKHR(vulkanInstance.presentQueue, &presentInfo);

		mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		vkQueueWaitIdle(vulkanInstance.presentQueue);

		return result;
	}

	void VulkanCommandBuffer::DestroyCommandPool(SVulkan &vulkanInstance)
	{
		vkDestroyCommandPool(vulkanInstance.logicalDevice, mCommandPool, nullptr);
		mCommandBuffers.clear();
	}

	void VulkanCommandBuffer::DestroySemaphores(SVulkan &vulkanInstance)
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