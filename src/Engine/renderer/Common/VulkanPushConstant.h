#ifndef VULKAN_PUSH_CONSTANT_H
#define VULKAN_PUSH_CONSTANT_H

#include "IPushConstant.h"
#include "renderer\Vulkan\VulkanBasePushConstant.h"

namespace Invision
{
	class VulkanInstance;

	class VulkanPushConstant : public IPushConstant
	{
	public:
		INVISION_API VulkanPushConstant() = delete;
		INVISION_API VulkanPushConstant(VulkanInstance* instance);

		INVISION_API VulkanPushConstant(VulkanInstance* instance, ShaderStageFlag shaderStages, uint32_t offset, uint32_t size);

		//INVISION_API ShaderStage GetShaderStages() override;
		INVISION_API uint32_t GetOffset() override;
		INVISION_API uint32_t GetSize() override;

		INVISION_API void SetShaderStages(ShaderStageFlag shaderStages) override;
		INVISION_API void SetOffset(uint32_t offset) override;
		INVISION_API void SetSize(uint32_t size) override;

		INVISION_API Invision::VulkanBasePushConstant GetBasePushConstant();

	private:
		Invision::VulkanInstance *mVulkanInstance;
		Invision::VulkanBasePushConstant mPushConstant;
	};

}

#endif // VULKAN_PUSH_CONSTANT_H