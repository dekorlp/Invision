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

		INVISION_API VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest);
		INVISION_API Invision::SVulkanContext& GetVulkanContext();
		INVISION_API void ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer) override;
		INVISION_API void ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest) override;


		INVISION_API std::shared_ptr<IRenderer> CreateRenderer() override;
		INVISION_API std::shared_ptr<IRenderPass> CreateRenderPass() override;
		INVISION_API std::shared_ptr<IVertexBuffer> CreateVertexBuffer() override;
		INVISION_API std::shared_ptr<IUniformBuffer> CreateUniformBuffer() override;
		INVISION_API std::shared_ptr<IPushConstant> CreatePushConstant(ShaderStageFlag shaderStages, uint32_t offset, uint32_t size) override;
		INVISION_API std::shared_ptr<IIndexBuffer> CreateIndexBuffer() override;
		INVISION_API std::shared_ptr<IPipeline> CreatePipeline() override;
		INVISION_API std::shared_ptr<IPipeline> CreatePipeline(PipelineProperties* pipelineProperties) override;
		INVISION_API std::shared_ptr<IFramebuffer> CreateFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass) override;
		INVISION_API std::shared_ptr<ICommandBuffer> CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer) override;
		INVISION_API std::shared_ptr<ITexture> CreateTexture() override;

		INVISION_API VulkanEngine* GetCoreEngine();
		INVISION_API VulkanBaseDepthRessources GetDepthRessources();
		INVISION_API VulkanBaseTexture GetColorRessources();
		INVISION_API unsigned int GetSizeSwapchainImages();

		INVISION_API ~VulkanInstance();

	private:
		VulkanEngine* mVulkanEngine;
		Invision::SVulkanContext mVulkanContext;
		Invision::VulkanBaseDepthRessources mDepthRessources;
		Invision::VulkanBaseTexture mColorRessources;

		// MainFramebuffer, MainRenderPass, MainCommandBuffer
		std::shared_ptr <Invision::IRenderPass> mMainRenderPass;
		std::shared_ptr <Invision::IFramebuffer> mMainFramebuffer;
		std::shared_ptr <Invision::ICommandBuffer> mMainCommandBuffer;

		bool mUseDepthTest = false;
	};

}

#endif // VULKAN_INSTANCE_H