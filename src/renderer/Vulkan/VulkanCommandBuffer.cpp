#include "precompiled.h"

#include "VulkanCommandBuffer.h"

namespace Invision
{
	VulkanCommandBuffer::VulkanCommandBuffer()
	{
		mCommandBufferIsInitialized = false;
		mIsCommandBufferRecording = false;
		mIsRenderPassStarted = false;
	}

	VulkanCommandBuffer::VulkanCommandBuffer(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, unsigned int countOfBuffers)
	{
		this->CreateCommandBuffer(vulkanInstance, commandPool, countOfBuffers);
		mIsCommandBufferRecording = false;
		mIsRenderPassStarted = false;
	}

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
		else
		{
			mCommandBufferIsInitialized = true;
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
			else
			{
				mIsCommandBufferRecording = true;
			}
		}
	}

	void VulkanCommandBuffer::SetViewport(VkViewport& viewport)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			{
				vkCmdSetViewport(mCommandBuffers[i], 0, 1, &viewport);
			}
		}
		else
		{
			throw VulkanException("Set Viewport cannot be executed!");
		}
	}

	void VulkanCommandBuffer::SetScissor(VkRect2D& scissor)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			{
				vkCmdSetScissor(mCommandBuffers[i], 0, 1, &scissor);
			}
		}
		else
		{
			throw VulkanException("Set Scissor cannot be executed!");
		}
	}

	void VulkanCommandBuffer::BeginRenderPass(SVulkan &vulkanInstance, VulkanRenderPass &renderPass, VulkanFramebuffer &vulkanFramebuffer)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
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

			mIsRenderPassStarted = true;
		}
		else
		{
			throw VulkanException("Begin Render Pass cannot be executed!");
		}
	}

	void VulkanCommandBuffer::BindPipeline(VulkanPipeline pipeline, VkPipelineBindPoint bindPoint)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			{
				vkCmdBindPipeline(mCommandBuffers[i], bindPoint, pipeline.GetPipeline());
			}
		}
		else
		{
			throw VulkanException("Bind Pipeline cannot be executed!");
		}
	}

	void VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording && mIsRenderPassStarted)
		{
			for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			{
				vkCmdDraw(mCommandBuffers[i], vertexCount, instanceCount, firstVertex, firstInstance);
			}
		}
		else
		{
			throw VulkanException("Draw Command cannot be executed!");
		}
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording && mIsRenderPassStarted)
		{
			for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			{
				vkCmdEndRenderPass(mCommandBuffers[i]);
			}
		}
		else
		{
			throw VulkanException("End Render Pass cannot be executed!");
		}
	}

	void VulkanCommandBuffer::EndCommandBuffer()
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			{
				if (vkEndCommandBuffer(mCommandBuffers[i]) != VK_SUCCESS)
				{
					throw VulkanException("failed to record command buffer!");
				}
			}
		}
		else
		{
			throw VulkanException("End Command Buffer cannot be executed!");
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