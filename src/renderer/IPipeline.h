#ifndef IPIPELINE_H
#define IPIPELINE_H


namespace Invision
{

	class IGraphicsEngine;
	class IUniformBuffer;
	class IVertexBuffer;
	class IRenderPass;

	enum ShaderStage;

	class IPipeline
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IPipeline() = delete;
		INVISION_API IPipeline(IGraphicsEngine* engine);
		INVISION_API virtual ~IPipeline() = default;

		INVISION_API IGraphicsEngine* engine();

		INVISION_API virtual void AddShader(const std::vector<char>& code, ShaderStage stage) = 0;
		INVISION_API virtual void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer) = 0;
		INVISION_API virtual void AddVertexBuffer(std::shared_ptr <Invision::IVertexBuffer> vertexBuffer) = 0;
		INVISION_API virtual void CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass) = 0;
	};
}

#endif // IPIPELINE_H