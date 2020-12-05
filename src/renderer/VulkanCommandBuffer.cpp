#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanPipeline.h"
#include "VulkanVertexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanIndexBuffer.h"

namespace Invision
{

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		ICommandBuffer(instance, framebuffer)
	{
		vulkanInstance = instance;
		
		commandBuffer.CreateCommandBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool(), (unsigned int)dynamic_pointer_cast<Invision::VulkanFramebuffer>(framebuffer)->GetFramebuffer().GetFramebuffers().size());
	}

	ICommandBuffer& VulkanCommandBuffer::BeginCommandBuffer()
	{
		commandBuffer.BeginCommandBuffer();
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


		commandBuffer.SetViewport(vkViewport);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::SetScissor(Invision::Rect rect)
	{
		VkRect2D vkRect;
		vkRect.offset.x = rect.left;
		vkRect.offset.y = rect.top;
		vkRect.extent.width = rect.right;
		vkRect.extent.height = rect.bottom;

		commandBuffer.SetScissor(vkRect);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BeginRenderPass(std::shared_ptr<IRenderPass> renderPass, std::shared_ptr<Invision::IFramebuffer> framebuffer)
	{
		std::vector< VkClearValue> clearValues = {};

		if (vulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
		{
			clearValues.resize(2);
			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clearValues[1].depthStencil = { 1.0, 0 };
		}
		else
		{
			clearValues.resize(1);
			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		commandBuffer.BeginRenderPass(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext(), dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass(), dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)->GetFramebuffer(), clearValues);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindPipeline(std::shared_ptr<IPipeline> pipeline)
	{
		commandBuffer.BindPipeline(dynamic_pointer_cast<VulkanPipeline>(pipeline)->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindVertexBuffer(std::vector<std::shared_ptr<IVertexBuffer>> vertexBuffer, uint32_t firstBinding, uint32_t bindingCount)
	{
		std::vector<VulkanBaseVertexBuffer> baseVertexBuffer;
		for (int i = 0; i < vertexBuffer.size(); i++)
		{
			baseVertexBuffer.push_back(dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer[i])->GetBaseVertexBuffer());
		}

		commandBuffer.BindVertexBuffer(baseVertexBuffer, firstBinding, bindingCount);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline)
	{
		commandBuffer.BindDescriptorSets(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer(), dynamic_pointer_cast<VulkanPipeline>(pipeline)->GetPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::BindIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer)
	{
		commandBuffer.BindIndexBuffer(dynamic_pointer_cast<VulkanIndexBuffer>(indexBuffer)->GetBuffer(), VK_INDEX_TYPE_UINT32);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		commandBuffer.Draw(vertexCount, instanceCount, firstVertex, firstInstance);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
	{
		commandBuffer.DrawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::EndRenderPass()
	{
		commandBuffer.EndRenderPass();
		return *this;
	}

	ICommandBuffer& VulkanCommandBuffer::EndCommandBuffer()
	{
		commandBuffer.EndCommandBuffer();
		return *this;
	}

	VulkanBaseCommandBuffer VulkanCommandBuffer::GetCommandBuffer()
	{
		return commandBuffer;
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		commandBuffer.DestroyCommandBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool());
	}

}