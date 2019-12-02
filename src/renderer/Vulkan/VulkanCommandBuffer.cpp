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

	void VulkanCommandBuffer::DestroyCommandPool(SVulkan &vulkanInstance)
	{
		vkDestroyCommandPool(vulkanInstance.logicalDevice, mCommandPool, nullptr);
	}

	void VulkanCommandBuffer::CreateCommandBuffers(SVulkan &vulkanInstance, VulkanFramebuffer &vulkanFramebuffer, VulkanPipeline &vulkanPipeline, VulkanRenderPass &renderPass)
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

	void  VulkanCommandBuffer::CreateSemaphores(SVulkan &vulkanInstance)
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(vulkanInstance.logicalDevice, &semaphoreInfo, nullptr, &mImageAvailableSemaphore) != VK_SUCCESS
			|| vkCreateSemaphore(vulkanInstance.logicalDevice, &semaphoreInfo, nullptr, &mRenderFinishedSemaphore) != VK_SUCCESS)
		{
			throw VulkanException("failed to create Semaphores");
		}
	}

	void VulkanCommandBuffer::DrawFrame(SVulkan &vulkanInstance)
	{
		uint32_t imageIndex;
		vkAcquireNextImageKHR(vulkanInstance.logicalDevice, vulkanInstance.swapChain, UINT64_MAX, mImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw VulkanException("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { vulkanInstance.swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional
		vkQueuePresentKHR(vulkanInstance.presentQueue, &presentInfo);
	}

	void VulkanCommandBuffer::DestroySemaphores(SVulkan &vulkanInstance)
	{
		vkDestroySemaphore(vulkanInstance.logicalDevice, mImageAvailableSemaphore, nullptr);
		vkDestroySemaphore(vulkanInstance.logicalDevice, mRenderFinishedSemaphore, nullptr);
	}
}