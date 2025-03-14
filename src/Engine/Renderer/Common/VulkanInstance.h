#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H


#include "IGraphicsInstance.h"
#include "renderer\Vulkan\VulkanBase.h"
#include "renderer\Vulkan\VulkanBaseDevice.h"
#include "renderer\Vulkan\VulkanBaseTexture.h"
#include "renderer\Vulkan\VulkanBaseCommandPool.h"
#include "renderer\Vulkan\VulkanBaseMemoryManager.h"

namespace Invision
{
	class VulkanEngine;
	enum MSAA;
	enum GfxFormat;

	class VulkanInstance : public IGraphicsInstance
	{
	public:

		INVISION_API VulkanInstance() = delete;

		INVISION_API VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, MSAAMode msaaMode);
		INVISION_API Invision::SVulkanContext& GetVulkanContext();
		INVISION_API void ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer) override;

		INVISION_API std::shared_ptr<IRenderer> CreateRenderer() override;
		INVISION_API std::shared_ptr<IRenderPass> CreateRenderPass() override;
		INVISION_API std::shared_ptr<IRenderPass> CreateDepthOnlyRenderPass(std::shared_ptr<ITexture>& attachment) override;
		INVISION_API std::shared_ptr<IVertexBindingDescription> CreateVertexBindingDescription() override;
		INVISION_API std::shared_ptr<IVertexBuffer> CreateVertexBuffer() override;
		INVISION_API std::shared_ptr<IUniformBuffer> CreateUniformBuffer() override;
		INVISION_API std::shared_ptr<IPushConstant> CreatePushConstant(ShaderStageFlag shaderStages, uint32_t offset, uint32_t size) override;
		INVISION_API std::shared_ptr<IIndexBuffer> CreateIndexBuffer() override;
		INVISION_API std::shared_ptr<IPipeline> CreatePipeline() override;
		INVISION_API std::shared_ptr<IPipeline> CreatePipeline(PipelineProperties* pipelineProperties) override;
		INVISION_API std::shared_ptr<IFramebuffer> CreateFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass, int width, int height) override;
		INVISION_API std::shared_ptr<ICommandBuffer> CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer) override;
		INVISION_API std::shared_ptr<ITexture> CreateTexture() override;
		INVISION_API std::shared_ptr<ITexture> CreateTexture(unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps) override;
		INVISION_API std::shared_ptr<ITexture> CreateColorAttachment(int width, int height, GfxFormat format);
		INVISION_API std::shared_ptr<ITexture> CreateDepthAttachment(int width, int height, bool isDepthStencil = false);

		INVISION_API VulkanEngine* GetCoreEngine();
		INVISION_API VulkanBaseTexture GetDepthRessources();
		INVISION_API VulkanBaseTexture GetColorRessources();
		INVISION_API unsigned int GetSizeSwapchainImages();

		// Helper Methods
		VkFormat ConvertInvisionFormatToVkFormat(GfxFormat format);

		INVISION_API Invision::VulkanBaseCommandPool GetCommandPool();
		INVISION_API Invision::VulkanBaseMemoryManager& GetMemoryManager();

		INVISION_API ~VulkanInstance();

	private:
		VulkanEngine* mVulkanEngine;

		Invision::VulkanBaseDevice mLogicalDevice;
		Invision::VulkanBasePresentation mPresentation;
		Invision::VulkanBaseCommandPool mCommandPool;
		Invision::VulkanBaseMemoryManager mMemoryManager;

		Invision::SVulkanContext mVulkanContext;
		Invision::VulkanBaseTexture mDepthRessources;
		Invision::VulkanBaseTexture mColorRessources;

		// MainFramebuffer, MainRenderPass, MainCommandBuffer
		std::shared_ptr <Invision::IRenderPass> mMainRenderPass;
		std::shared_ptr <Invision::IFramebuffer> mMainFramebuffer;
		std::shared_ptr <Invision::ICommandBuffer> mMainCommandBuffer;

		void UpdateDepthTexture();
		void UpdateMSAATexture();
		void ActivateMSAA(MSAAMode msaa);

		// check MSAA
		VkSampleCountFlagBits GetMaxUsableSampleCount(SVulkanBase& vulkanInstance);
		VkSampleCountFlagBits IsMSAASampleSupported(SVulkanBase& vulkanInstance, VkSampleCountFlagBits flags);
	};

}

#endif // VULKAN_INSTANCE_H