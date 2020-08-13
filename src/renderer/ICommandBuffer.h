#ifndef ICOMMAND_BUFFER_H
#define ICOMMAND_BUFFER_H


namespace Invision
{

	class IGraphicsEngine;
	class IGraphicsInstance;

	class IFramebuffer;
	class IRenderPass;
	class IPipeline;
	class IVertexBuffer;
	class IUniformBuffer;
	class IIndexBuffer;

	struct Viewport;
	struct Rect;

	class ICommandBuffer
	{
		IGraphicsEngine* Engine = nullptr;
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API ICommandBuffer() = delete;
		INVISION_API ICommandBuffer(IGraphicsInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer);

		INVISION_API virtual ICommandBuffer& BeginCommandBuffer() = 0;
		INVISION_API virtual ICommandBuffer& SetViewport(Invision::Viewport viewport) = 0;
		INVISION_API virtual ICommandBuffer& SetScissor(Invision::Rect rect) = 0;
		INVISION_API virtual ICommandBuffer& BeginRenderPass(std::shared_ptr<IRenderPass> renderPass, std::shared_ptr<Invision::IFramebuffer> framebuffer) = 0;
		INVISION_API virtual ICommandBuffer& BindPipeline(std::shared_ptr<IPipeline> pipeline) = 0;
		INVISION_API virtual ICommandBuffer& BindVertexBuffer(std::vector< std::shared_ptr<IVertexBuffer>> vertexBuffer, uint32_t firstBinding, uint32_t bindingCount) = 0;
		INVISION_API virtual ICommandBuffer& BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline) = 0;
		INVISION_API virtual ICommandBuffer& BindIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer) = 0;
		INVISION_API virtual ICommandBuffer& Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
		INVISION_API virtual ICommandBuffer& DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0) = 0;
		INVISION_API virtual ICommandBuffer& EndRenderPass() = 0;
		INVISION_API virtual ICommandBuffer& EndCommandBuffer() = 0;
	

		INVISION_API virtual ~ICommandBuffer() = default;
		INVISION_API IGraphicsEngine* engine();
		

	};
}

#endif // ICOMMAND_BUFFER_H
