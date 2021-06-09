#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H


#include "IPipeline.h"
#include "renderer\Vulkan\VulkanBaseShader.h"
#include "renderer\Vulkan\VulkanBasePipeline.h"

namespace Invision
{
	class VulkanInstance;
	class VulkanUniformBuffer;

	class VulkanPipeline : public IPipeline
	{
	

	public:

		INVISION_API VulkanPipeline() = delete;

		INVISION_API VulkanPipeline(VulkanInstance* instance);
		INVISION_API VulkanPipeline(VulkanInstance* instance, PipelineProperties* pipelineProperties);

		INVISION_API void AddShader(const std::vector<char>& code, ShaderStage stage);

		INVISION_API void ClearUniformBuffer();
		INVISION_API void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer);
		INVISION_API void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, uint32_t set);

		INVISION_API void AddVertexBuffer(std::shared_ptr<Invision::IVertexBindingDescription> vertexBindingDescription);

		INVISION_API void BindPushConstant(std::shared_ptr <Invision::IPushConstant> pushConstant);

		INVISION_API void CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass);

		INVISION_API VulkanBasePipeline GetPipeline();

		INVISION_API ~VulkanPipeline();


	private:
		Invision::VulkanInstance *mVulkanInstance;

		std::vector< Invision::VulkanBaseShader> mShaders;
		Invision::VulkanBasePipeline mPipeline;

		std::shared_ptr<PipelineProperties> mPipelineProperties = nullptr;

	};

}

#endif // VULKAN_PIPELINE_H