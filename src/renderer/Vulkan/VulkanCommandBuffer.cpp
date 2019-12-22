#include "precompiled.h"

#include "VulkanCommandBuffer.h"

namespace Invision
{
	void VulkanCommandBuffer::CreateCommandBuffers(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, VulkanFramebuffer &vulkanFramebuffer, VulkanPipeline &vulkanPipeline, VulkanRenderPass &renderPass, VkViewport viewportParam, VkRect2D scissorParam)
	{
		mCommandBuffers.clear();
		mCommandBuffers.resize(vulkanFramebuffer.GetFramebuffers().size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.GetCommandPool();
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
				scissor.extent.width = vulkanInstance.swapChainExtent.width;
				scissor.extent.height = vulkanInstance.swapChainExtent.height;
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

	std::vector<VkCommandBuffer> VulkanCommandBuffer::GetCommandBuffers()
	{
		return mCommandBuffers;
	}

	VkCommandBuffer* VulkanCommandBuffer::GetCommandBuffer(int index)
	{
		return &mCommandBuffers[index];
	}
}