#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseFramebuffer.h"
#include "VulkanBasePipeline.h"
#include "VulkanBaseRenderPass.h"
#include "VulkanBaseVertexBuffer.h"
#include "VulkanBaseIndexBuffer.h"
#include "VulkanBaseUniformBuffer.h"
#include "VulkanBasePushConstant.h"
#include "VulkanBaseCommandPool.h"
#include "VulkanBaseCommandBuffer.h"
#include "VulkanBaseMemoryManager.h"

namespace Invision
{
	VulkanBaseCommandBuffer::VulkanBaseCommandBuffer()
	{
		mCommandBufferIsInitialized = false;
		mIsCommandBufferRecording = false;
		mIsRenderPassStarted = false;
		mIsIndexBufferBinded = false;
		mIsVertexBufferBinded = false;
	}

	VulkanBaseCommandBuffer::VulkanBaseCommandBuffer(SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, unsigned int countOfBuffers)
	{
		this->CreateCommandBuffer(vulkanInstance, commandPool, countOfBuffers);
		mIsCommandBufferRecording = false;
		mIsRenderPassStarted = false;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::CreateCommandBuffer(SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, unsigned int countOfBuffers)
	{
		//mCommandBuffers.clear();
		//mCommandBuffers.resize(countOfBuffers);

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(vulkanInstance.logicalDevice, &allocInfo, &mCommandBuffer) != VK_SUCCESS)
		{
			throw VulkanBaseException("failed to create command buffers!");
		}
		else
		{
			mCommandBufferIsInitialized = true;
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BeginCommandBuffer()
	{
		//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
		//{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS)
			{
				throw VulkanBaseException("failed to begin recording command buffer!");
			}
		//}
		mIsCommandBufferRecording = true;

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::SetViewport(VkViewport& viewport)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
			//}
		}
		else
		{
			throw VulkanBaseException("Set Viewport cannot be executed!");
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::SetScissor(VkRect2D& scissor)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffer.size(); i++)
			//{
				vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);
			//}
		}
		else
		{
			throw VulkanBaseException("Set Scissor cannot be executed!");
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BeginRenderPass(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, VulkanBaseFramebuffer &vulkanFramebuffer, unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, std::vector<VkClearValue> clearValues)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPass.GetRenderPass();
				renderPassInfo.framebuffer = vulkanFramebuffer.GetFramebuffers();
				renderPassInfo.renderArea.offset.x = offsetX;
				renderPassInfo.renderArea.offset.y = offsetY;
				renderPassInfo.renderArea.extent.width = width;
				renderPassInfo.renderArea.extent.height = height;
				renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();


				vkCmdBeginRenderPass(mCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			//}

			mIsRenderPassStarted = true;
		}
		else
		{
			throw VulkanBaseException("Begin Render Pass cannot be executed!");
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BindPipeline(VulkanBasePipeline &pipeline, VkPipelineBindPoint bindPoint)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdBindPipeline(mCommandBuffer, bindPoint, pipeline.GetPipeline());
			//}
		}
		else
		{
			throw VulkanBaseException("Bind Pipeline cannot be executed!");
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BindVertexBuffer(std::vector<VulkanBaseVertexBuffer> vertexBuffers, VulkanBaseMemoryManager& memoryManager, uint32_t firstBinding, uint32_t bindingCount)
	{
		std::vector<VkBuffer> bindingBuffers;
		std::vector<VkDeviceSize> bindingOffsets;

		//VulkanBaseBuffer
		// fill binding Buffers
		for (int i = 0; i < vertexBuffers[0].GetBuffers().size(); i++)
		{
			bindingBuffers.push_back(memoryManager.GetBuffer(vertexBuffers[0].GetBuffers()[i]));
			bindingOffsets.push_back(memoryManager.GetOffset(vertexBuffers[0].GetBuffers()[i]));
		}

		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdBindVertexBuffers(mCommandBuffer, firstBinding, bindingCount, bindingBuffers.data(), bindingOffsets.data());
			//}
		}
		else
		{
			throw VulkanBaseException("Bind Vertex Buffer cannot be executed!");
		}

		mIsVertexBufferBinded = true;

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BindIndexBuffer(VulkanBaseIndexBuffer &indexBuffer, VkIndexType indexType)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdBindIndexBuffer(mCommandBuffer, indexBuffer.GetBuffer(), indexBuffer.GetOffset(), indexType);
			//}
		}
		else
		{
			throw VulkanBaseException("Bind Index Buffer cannot be executed!");
		}

		mIsIndexBufferBinded = true;

		return *this;
	}
	
	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BindDescriptorSets(VulkanBaseUniformBuffer &uniformBuffer, VulkanBasePipeline& pipeline, VkPipelineBindPoint bindPoint)
	{
		std::vector<VkDescriptorSet> sets;
		for (int i = 0; i < uniformBuffer.GetSets().size(); i++)
		{
			sets.push_back(uniformBuffer.GetSets()[i].mDescriptorSet);
		}


		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int j = 0; j < mCommandBuffers.size(); j++)
			//{
				vkCmdBindDescriptorSets(mCommandBuffer, bindPoint, pipeline.GetPipelineLayout(), 0, static_cast<uint32>(sets.size()), sets.data(), 0, nullptr);
			//}
		}
		else
		{
			throw VulkanBaseException("Bind Descriptor Sets cannot be executed!");
		}
		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::BindDescriptorSets(VulkanBaseUniformBuffer &uniformBuffer, VulkanBasePipeline& pipeline, VkPipelineBindPoint bindPoint, uint32_t set)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int j = 0; j < mCommandBuffers.size(); j++)
			//{
				vkCmdBindDescriptorSets(mCommandBuffer, bindPoint, pipeline.GetPipelineLayout(), set, 1, &uniformBuffer.GetSets()[set].mDescriptorSet, 0, nullptr);
			//}
		}
		else
		{
			throw VulkanBaseException("Bind Descriptor Sets cannot be executed!");
		}
		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::PushConstant(VulkanBasePushConstant pushConstant, VulkanBasePipeline& pipeline, const void* data)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdPushConstants(mCommandBuffer, pipeline.GetPipelineLayout(), pushConstant.GetShaderStages(), static_cast<uint32>(pushConstant.GetOffset()), static_cast<uint32>(pushConstant.GetSize()), data);
			//}
		}
		else
		{
			throw VulkanBaseException("Bind Descriptor Sets cannot be executed!");
		}
		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording && mIsRenderPassStarted)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
			//}
		}
		else
		{
			if (!mIsVertexBufferBinded)
			{
				throw VulkanBaseException("Draw Command cannot be executed, bind first an Vertex Buffer!");
			}
			else
			{
				throw VulkanBaseException("Draw Command cannot be executed!");
			}
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording && mIsRenderPassStarted && mIsIndexBufferBinded && mIsVertexBufferBinded)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
			//}
		}
		else
		{
			if (!mIsIndexBufferBinded)
			{
				throw VulkanBaseException("Draw Indexed Command cannot be executed, bind first an Index Buffer!");
			}
			else if (!mIsVertexBufferBinded)
			{
				throw VulkanBaseException("Draw Indexed Command cannot be executed, bind first an Vertex Buffer!");
			}
			else
			{
				throw VulkanBaseException("Draw Indexed Command cannot be executed!");
			}
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::EndRenderPass()
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording && mIsRenderPassStarted)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				vkCmdEndRenderPass(mCommandBuffer);
			//}
		}
		else
		{
			throw VulkanBaseException("End Render Pass cannot be executed!");
		}

		return *this;
	}

	VulkanBaseCommandBuffer& VulkanBaseCommandBuffer::EndCommandBuffer()
	{
		if (mCommandBufferIsInitialized && mIsCommandBufferRecording)
		{
			//for (unsigned int i = 0; i < mCommandBuffers.size(); i++)
			//{
				if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS)
				{
					throw VulkanBaseException("failed to record command buffer!");
				}
			//}
		}
		else
		{
			throw VulkanBaseException("End Command Buffer cannot be executed!");
		}

		return *this;
	}

	const VkCommandBuffer* VulkanBaseCommandBuffer::GetCommandBuffer()
	{
		return &mCommandBuffer;
	}

	void VulkanBaseCommandBuffer::DestroyCommandBuffer(SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool)
	{
		vkFreeCommandBuffers(vulkanInstance.logicalDevice, commandPool.GetCommandPool(), 1, &mCommandBuffer);
	}
}