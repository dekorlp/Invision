#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H


#include "IGraphicsInstance.h"
#include "renderer\Vulkan\VulkanBase.h"
#include "renderer\Vulkan\VulkanBaseDepthRessources.h"

namespace Invision
{
	class VulkanEngine;

	class VulkanInstance : public IGraphicsInstance
	{
	public:

		INVISION_API VulkanInstance() = delete;

		INVISION_API VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, bool activateDepthTest);
		INVISION_API Invision::SVulkanContext& GetVulkanContext();
		INVISION_API void ResetPresentation(CanvasDimensions canvas) override;
		INVISION_API void ResetPresentation(CanvasDimensions canvas, bool activateDepthTest) override;


		INVISION_API std::shared_ptr<IRenderer> CreateRenderer() override;
		INVISION_API std::shared_ptr<IRenderPass> CreateRenderPass() override;
		INVISION_API std::shared_ptr<IVertexBuffer> CreateVertexBuffer() override;
		INVISION_API std::shared_ptr<IUniformBuffer> CreateUniformBuffer() override;
		INVISION_API std::shared_ptr<IPushConstant> CreatePushConstant(ShaderStage shaderStages, uint32_t offset, uint32_t size) override;
		INVISION_API std::shared_ptr<IIndexBuffer> CreateIndexBuffer() override;
		INVISION_API std::shared_ptr<IPipeline> CreatePipeline() override;
		INVISION_API std::shared_ptr<IPipeline> CreatePipeline(PipelineProperties* pipelineProperties) override;
		INVISION_API std::shared_ptr<IFramebuffer> CreateFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass, unsigned int countFrameBuffers) override;
		INVISION_API std::shared_ptr<ICommandBuffer> CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer) override;
		INVISION_API std::shared_ptr<ITexture> CreateTexture() override;

		INVISION_API VulkanEngine* GetCoreEngine();
		INVISION_API VulkanBaseDepthRessources GetDepthRessources();
		INVISION_API unsigned int GetSizeSwapchainImages();

		INVISION_API ~VulkanInstance();

	private:
		VulkanEngine* vulkanEngine;
		Invision::SVulkanContext vulkanContext;
		Invision::VulkanBaseDepthRessources depthRessources;

		bool mUseDepthTest = false;
	};

}

#endif // VULKAN_INSTANCE_H