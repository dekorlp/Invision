#ifndef IUNIFORM_BUFFER_H
#define IUNIFORM_BUFFER_H


namespace Invision
{

	class IGraphicsEngine;


	enum ShaderStage;
	


	class IUniformBuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IUniformBuffer() = delete;
		INVISION_API IUniformBuffer(IGraphicsEngine* engine);
		INVISION_API virtual IUniformBuffer& CreateUniformBinding(uint32_t binding, uint32_t descriptorCount, ShaderStage shaderStage, uint64_t bufferSize, uint64_t offset) = 0;
		INVISION_API virtual void CreateUniformBuffer() = 0;
		INVISION_API virtual void UpdateUniform(const void* source, size_t size, uint32_t binding) = 0;
		INVISION_API virtual ~IUniformBuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IUNIFORM_BUFFER_H
