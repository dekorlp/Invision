#ifndef ICOMMAND_BUFFER_H
#define ICOMMAND_BUFFER_H


namespace Invision
{
	class IGraphicsInstance;

	class IFramebuffer;
	class IRenderPass;
	class IPipeline;
	class IVertexBuffer;
	class IUniformBuffer;
	class IIndexBuffer;
	class IPushConstant;

	struct Viewport;
	struct Rect;

	enum IndexType;

	class ICommandBuffer
	{
		IGraphicsInstance* mInstance = nullptr;

	public:
		INVISION_API ICommandBuffer() = delete;
		INVISION_API ICommandBuffer(IGraphicsInstance* instance, std::shared_ptr<Invision::IFramebuffer> framebuffer);

		INVISION_API virtual ICommandBuffer& BeginCommandBuffer() = 0;
		INVISION_API virtual ICommandBuffer& SetViewport(Invision::Viewport viewport) = 0;
		INVISION_API virtual ICommandBuffer& SetScissor(Invision::Rect rect) = 0;
		INVISION_API virtual ICommandBuffer& SetBackground(float r, float g, float b, float a) = 0;
		INVISION_API virtual ICommandBuffer& BeginRenderPass(std::shared_ptr<IRenderPass> renderPass, std::shared_ptr<Invision::IFramebuffer> framebuffer) = 0;
		INVISION_API virtual ICommandBuffer& BindPipeline(std::shared_ptr<IPipeline> pipeline) = 0;
		INVISION_API virtual ICommandBuffer& BindVertexBuffer(std::vector< std::shared_ptr<IVertexBuffer>> vertexBuffer, uint32_t firstBinding, uint32_t bindingCount) = 0;
		INVISION_API virtual ICommandBuffer& BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline) = 0;
		INVISION_API virtual ICommandBuffer& BindDescriptorSets(std::shared_ptr<IUniformBuffer> uniformBuffer, std::shared_ptr<IPipeline> pipeline, uint32_t set) = 0;
		INVISION_API virtual ICommandBuffer& BindIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer, IndexType indexType) = 0;
		INVISION_API virtual ICommandBuffer& PushConstant(std::shared_ptr<IPushConstant> pushConstant, std::shared_ptr<IPipeline> pipeline, const void* data) = 0;
		INVISION_API virtual ICommandBuffer& Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
		INVISION_API virtual ICommandBuffer& DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0) = 0;
		INVISION_API virtual ICommandBuffer& EndRenderPass() = 0;
		INVISION_API virtual ICommandBuffer& EndCommandBuffer() = 0;
	

		INVISION_API virtual ~ICommandBuffer() = default;
	};
}

#endif // ICOMMAND_BUFFER_H
