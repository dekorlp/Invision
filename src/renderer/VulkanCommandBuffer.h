#ifndef VULKAN_COMMAND_BUFFER_H
#define VULKAN_COMMAND_BUFFER_H


#include "ICommandBuffer.h"
#include "renderer\Vulkan\VulkanBaseCommandBuffer.h"

namespace Invision
{
	class VulkanEngine;
	class VulkanCommandBuffer : public ICommandBuffer
	{
	public:

		INVISION_API VulkanCommandBuffer() = delete;
		INVISION_API VulkanCommandBuffer(VulkanEngine* engine, std::shared_ptr<Invision::IFramebuffer> framebuffer);

		ICommandBuffer& BeginCommandBuffer() override;
		ICommandBuffer& SetViewport(Invision::Viewport viewport) override;
		ICommandBuffer& SetScissor(Invision::Rect rect) override;
		ICommandBuffer& BeginRenderPass(std::shared_ptr<IRenderPass> renderPass, std::shared_ptr<Invision::IFramebuffer> framebuffer) override;
		ICommandBuffer& BindPipeline(std::shared_ptr<IPipeline> pipeline) override;
		ICommandBuffer& BindVertexBuffer(std::vector< std::shared_ptr<IVertexBuffer>> vertexBuffer, uint32_t firstBinding, uint32_t bindingCount) override;
		ICommandBuffer& BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline) override;
		ICommandBuffer& BindIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer) override;
		ICommandBuffer& Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) override;
		ICommandBuffer& DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0) override;
		ICommandBuffer& EndRenderPass() override;
		ICommandBuffer& EndCommandBuffer() override;

		VulkanBaseCommandBuffer GetCommandBuffer();

		~VulkanCommandBuffer();

	private:
		Invision::VulkanEngine *vulkanEngine;
		Invision::VulkanBaseCommandBuffer commandBuffer;

	};

}

#endif // VULKAN_COMMAND_BUFFER_H