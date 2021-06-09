#ifndef IPIPELINE_H
#define IPIPELINE_H


namespace Invision
{
	class IGraphicsInstance;

	class IUniformBuffer;
	class IVertexBuffer;
	class IVertexBindingDescription;
	class IPushConstant;
	class IRenderPass;

	struct PipelineProperties;

	enum ShaderStage;

	enum PrimitiveTopology;
	enum PolygoneMode;
	enum CullMode;
	enum FrontFaceMode;


	class IPipeline
	{
		IGraphicsInstance* mInstance = nullptr;

	public:
		INVISION_API IPipeline() = delete;
		INVISION_API IPipeline(IGraphicsInstance* instance);
		INVISION_API IPipeline(IGraphicsInstance* instance, PipelineProperties* pipelineProperties);
		INVISION_API virtual ~IPipeline() = default;


		INVISION_API virtual void AddShader(const std::vector<char>& code, ShaderStage stage) = 0;
		INVISION_API virtual void ClearUniformBuffer() = 0;
		INVISION_API virtual void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer) = 0;
		INVISION_API virtual void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, uint32_t set) = 0;
		INVISION_API virtual void AddVertexBuffer(std::shared_ptr <Invision::IVertexBindingDescription> vertexBinding) = 0;
		INVISION_API virtual void BindPushConstant(std::shared_ptr <Invision::IPushConstant> pushConstant) = 0;
		INVISION_API virtual void CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass) = 0;
	};
}

#endif // IPIPELINE_H