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
	enum BlendFactor;
	enum BlendOp;
	enum CompareOp;


	class IPipeline
	{
		IGraphicsInstance* mInstance = nullptr;

	public:
		INVISION_API IPipeline() = delete;
		INVISION_API IPipeline(IGraphicsInstance* instance);
		INVISION_API IPipeline(IGraphicsInstance* instance, PipelineProperties* pipelineProperties);
		INVISION_API virtual ~IPipeline() = default;


		INVISION_API virtual void AddShader(const std::vector<char>& code, ShaderStage stage) = 0;
		INVISION_API virtual void SetColorBlendFunction(bool blendEnable, BlendFactor srcColorBlendFactor = (BlendFactor)0, BlendFactor dstColorBlendFactor = BlendFactor(0), BlendOp colorBlendOp = BlendOp(0), BlendFactor srcAlphaBlendFactor = BlendFactor(0), BlendFactor dstAlphaBlendFactor = BlendFactor(0), BlendOp alphaBlendOp = BlendOp(0)) = 0;
		INVISION_API virtual void SetDepthTest(bool enable, CompareOp compareOp = (CompareOp)1) = 0;
		INVISION_API virtual void SetDepthWrite(bool enable) = 0;
		INVISION_API virtual void ClearUniformBuffer() = 0;
		INVISION_API virtual void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer) = 0;
		INVISION_API virtual void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, uint32_t set) = 0;
		INVISION_API virtual void AddVertexDescription(std::shared_ptr <Invision::IVertexBindingDescription> vertexBinding) = 0;
		INVISION_API virtual void BindPushConstant(std::shared_ptr <Invision::IPushConstant> pushConstant) = 0;
		INVISION_API virtual void CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass) = 0;
	};
}

#endif // IPIPELINE_H