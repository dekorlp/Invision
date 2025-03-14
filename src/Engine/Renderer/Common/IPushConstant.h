#ifndef I_PUSH_CONSTANT_H
#define I_PUSH_CONSTANT_H


namespace Invision
{
	class IGraphicsInstance;

	enum ShaderStage;
	typedef uint32_t ShaderStageFlag;

	class IPushConstant
	{
		IGraphicsInstance* mInstance = nullptr;
	public:
		INVISION_API IPushConstant() = delete;
		INVISION_API IPushConstant(IGraphicsInstance* instance);
		INVISION_API virtual ~IPushConstant() = default;

		INVISION_API IPushConstant(IGraphicsInstance* instance, ShaderStageFlag shaderStages, uint32_t offset, uint32_t size);

		//INVISION_API virtual ShaderStage GetShaderStages() = 0;
		INVISION_API virtual uint32_t GetOffset() = 0;
		INVISION_API virtual uint32_t GetSize() = 0;

		INVISION_API virtual void SetShaderStages(ShaderStageFlag shaderStages) = 0;
		INVISION_API virtual void SetOffset(uint32_t offset) = 0;
		INVISION_API virtual void SetSize(uint32_t size) = 0;
	};

}

#endif // I_PUSH_CONSTANT_H