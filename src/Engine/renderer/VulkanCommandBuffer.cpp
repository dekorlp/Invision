#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanPipeline.h"
#include "VulkanVertexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanPushConstant.h"
#include "VulkanIndexBuffer.h"

namespace Invision
{

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		ICommandBuffer(instance, framebuffer)
	{
		mVulkanInstance = instance;
		mCommandBuffers.resize(dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)->GetFramebuffers().size());

		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].CreateCommandBuffer(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetCoreEngine()->GetCommandPool(), (unsigned int)dynamic_pointer_cast<Invision::VulkanFramebuffer>(framebuffer)->GetFramebuffers().size());
		}
	}

	ICommandBuffer& VulkanCommandBuffer::BeginCommandBuffer()
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].BeginCommandBuffer();
		}
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::SetViewport(Invision::Viewport viewport)
	{
		VkViewport vkViewport;
		vkViewport.x = viewport.TopLeftX;
		vkViewport.y = viewport.TopLeftY;
		vkViewport.width = (float)viewport.Width;
		vkViewport.height = (float)viewport.Height;
		vkViewport.minDepth = viewport.MinDepth;
		vkViewport.maxDepth = viewport.MaxDepth;

		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].SetViewport(vkViewport);
		}
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::SetScissor(Invision::Rect rect)
	{
		VkRect2D vkRect;
		vkRect.offset.x = rect.left;
		vkRect.offset.y = rect.top;
		vkRect.extent.width = rect.right;
		vkRect.extent.height = rect.bottom;

		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].SetScissor(vkRect);
		}
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::SetBackground(float r, float g, float b, float a)
	{
		mBackground[0] = r;
		mBackground[1] = g;
		mBackground[2] = b;
		mBackground[3] = a;

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BeginRenderPass(std::shared_ptr<IRenderPass> renderPass, std::shared_ptr<Invision::IFramebuffer> framebuffer)
	{
		std::vector< VkClearValue> clearValues = {};

		if (mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
		{
			clearValues.resize(2);
			clearValues[0].color = { mBackground[0], mBackground[1], mBackground[2], mBackground[3] };
			clearValues[1].depthStencil = { 1.0, 0 };
		}
		else
		{
			clearValues.resize(1);
			clearValues[0].color = { mBackground[0], mBackground[1], mBackground[2], mBackground[3] };
		}

		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].BeginRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass(), dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)->GetFramebuffer(i), clearValues);
		}
		
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindPipeline(std::shared_ptr<IPipeline> pipeline)
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].BindPipeline(dynamic_pointer_cast<VulkanPipeline>(pipeline)->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindVertexBuffer(std::vector<std::shared_ptr<IVertexBuffer>> vertexBuffer, uint32_t firstBinding, uint32_t bindingCount)
	{
		std::vector<VulkanBaseVertexBuffer> baseVertexBuffer;
		for (int i = 0; i < vertexBuffer.size(); i++)
		{
			baseVertexBuffer.push_back(dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer[i])->GetBaseVertexBuffer());
		}

		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			
			mCommandBuffers[i].BindVertexBuffer(baseVertexBuffer, mVulkanInstance->GetCoreEngine()->GetMemoryManager(), firstBinding, bindingCount);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline)
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].BindDescriptorSets(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer(), dynamic_pointer_cast<VulkanPipeline>(pipeline)->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline, uint32_t set)
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].BindDescriptorSets(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer(), dynamic_pointer_cast<VulkanPipeline>(pipeline)->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS, set);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::PushConstant(std::shared_ptr<IPushConstant> pushConstant, std::shared_ptr<IPipeline> pipeline, const void* data)
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].PushConstant(dynamic_pointer_cast<VulkanPushConstant>(pushConstant)->GetBasePushConstant(), dynamic_pointer_cast<VulkanPipeline>(pipeline)->GetPipeline(), data);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer, IndexType indexType)
	{
		VkIndexType vkIndexType;

	
		switch (indexType)
		{
		case INDEX_TYPE_UINT16:
			vkIndexType = VK_INDEX_TYPE_UINT16;
			break;
		case INDEX_TYPE_UINT32:
			vkIndexType = VK_INDEX_TYPE_UINT32;
			break;
		default:
			throw InvisionBaseRendererException("Unknown Index Type passed to Function BindIndexBuffer");

		}

		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].BindIndexBuffer(dynamic_pointer_cast<VulkanIndexBuffer>(indexBuffer)->GetBuffer(), vkIndexType);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].Draw(vertexCount, instanceCount, firstVertex, firstInstance);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].DrawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::EndRenderPass()
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].EndRenderPass();
		}

		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::EndCommandBuffer()
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].EndCommandBuffer();
		}

		return *this;
	}

	VulkanBaseCommandBuffer VulkanCommandBuffer::GetCommandBuffer(unsigned int index)
	{
		return mCommandBuffers[index];
	}

	unsigned int VulkanCommandBuffer::GetCountOfCommandBuffers()
	{
		return static_cast<unsigned int>(mCommandBuffers.size());
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		for (int i = 0; i < mCommandBuffers.size(); i++)
		{
			mCommandBuffers[i].DestroyCommandBuffer(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetCoreEngine()->GetCommandPool());
		}
	}

}