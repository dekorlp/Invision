#ifndef IUNIFORM_BUFFER_H
#define IUNIFORM_BUFFER_H


namespace Invision
{
	class IGraphicsInstance;

	class ITexture;

	enum ShaderStage;
	
	class IUniformBuffer
	{
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IUniformBuffer() = delete;
		INVISION_API IUniformBuffer(IGraphicsInstance* instance);
		INVISION_API virtual IUniformBuffer& CreateUniformBinding(uint32_t set, uint32_t binding, uint32_t descriptorCount, ShaderStage shaderStage, uint64_t bufferSize, uint64_t offset) = 0;
		INVISION_API virtual IUniformBuffer& CreateImageBinding(uint32_t set, uint32_t binding, uint32_t descriptorCount, ShaderStage shaderStage, std::shared_ptr < Invision::ITexture> texture) = 0;
		INVISION_API virtual void CreateUniformBuffer() = 0;
		INVISION_API virtual void UpdateUniform(const void* source, size_t size, uint32_t set, uint32_t binding) = 0;
		INVISION_API virtual ~IUniformBuffer() = default;
	};
}

#endif // IUNIFORM_BUFFER_H
