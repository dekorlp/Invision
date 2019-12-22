#include "precompiled.h"

#include "VulkanCommandBuffer.h"

namespace Invision
{
	void VulkanCommandBuffer::CreateCommandBuffer(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, unsigned int countOfBuffers)
	{
		mCommandBuffers.clear();
		mCommandBuffers.resize(countOfBuffers);

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32)mCommandBuffers.size();

		if (vkAllocateCommandBuffers(vulkanInstance.logicalDevice, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
		{
			throw VulkanException("failed to create command buffers!");
		}
	}

	void VulkanCommandBuffer::BeginCommandBuffer()
	{
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
		}
	}

	void VulkanCommandBuffer::SetViewport(VkViewport& viewport)
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			vkCmdSetViewport(mCommandBuffers[i], 0, 1, &viewport);
		}
	}

	void VulkanCommandBuffer::SetScissor(VkRect2D& scissor)
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			vkCmdSetScissor(mCommandBuffers[i], 0, 1, &scissor);
		}
	}

	void VulkanCommandBuffer::BeginRenderPass(SVulkan &vulkanInstance, VulkanRenderPass &renderPass, VulkanFramebuffer &vulkanFramebuffer)
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass.GetRenderPass();
			renderPassInfo.framebuffer = vulkanFramebuffer.GetFramebuffers()[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = vulkanInstance.swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;
		
			vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		}
	}

	void VulkanCommandBuffer::BindPipeline(VulkanPipeline pipeline, VkPipelineBindPoint bindPoint)
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			vkCmdBindPipeline(mCommandBuffers[i], bindPoint, pipeline.GetPipeline());
		}
	}

	void VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			vkCmdDraw(mCommandBuffers[i], vertexCount, instanceCount, firstVertex, firstInstance);
		}
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
			vkCmdEndRenderPass(mCommandBuffers[i]);
		}
	}

	void VulkanCommandBuffer::EndCommandBuffer()
	{
		for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		{
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