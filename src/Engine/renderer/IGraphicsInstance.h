#ifndef IGRAPHICS_INSTANCE_H
#define IGRAPHICS_INSTANCE_H

#include "IRenderer.h"
#include "IRenderPass.h"
#include "IVertexBuffer.h"
#include "IUniformBuffer.h"
#include "IIndexBuffer.h"
#include "IPipeline.h"
#include "IFramebuffer.h"
#include "ICommandBuffer.h"
#include "IPushConstant.h"
#include "ITexture.h"

namespace Invision
{

	class IGraphicsEngine;
	struct CanvasDimensions;
	enum MSAAMode;

	class IGraphicsInstance
	{
		IGraphicsEngine* mEngine = nullptr;

	public:
		INVISION_API IGraphicsInstance() = delete;
		INVISION_API IGraphicsInstance(IGraphicsEngine* engine);
		INVISION_API IGraphicsInstance(IGraphicsEngine* engine, CanvasDimensions* canvas, bool activateDepthTest, MSAAMode msaaMode);
		INVISION_API virtual void ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer) = 0;

		INVISION_API virtual std::shared_ptr<IRenderer> CreateRenderer() = 0;
		INVISION_API virtual std::shared_ptr<IRenderPass> CreateRenderPass() = 0;
		INVISION_API virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer() = 0;
		INVISION_API virtual std::shared_ptr<IUniformBuffer> CreateUniformBuffer() = 0;
		INVISION_API virtual std::shared_ptr<IPushConstant> CreatePushConstant(ShaderStageFlag shaderStages, uint32_t offset, uint32_t size) = 0;
		INVISION_API virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer() = 0;
		INVISION_API virtual std::shared_ptr<IPipeline> CreatePipeline() = 0;
		INVISION_API virtual std::shared_ptr<IPipeline> CreatePipeline(PipelineProperties* pipelineProperties) = 0;
		INVISION_API virtual std::shared_ptr<IFramebuffer> CreateFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass) = 0;
		INVISION_API virtual std::shared_ptr<ICommandBuffer> CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer) = 0;
		INVISION_API virtual std::shared_ptr<ITexture> CreateTexture() = 0;
		INVISION_API virtual std::shared_ptr<ITexture> CreateTexture(unsigned char* pixels, int width, int height, bool generateMipMaps = false) = 0;
		INVISION_API virtual unsigned int GetSizeSwapchainImages() = 0;

		INVISION_API virtual void ActivateDepthTesting(bool activateDepthTesting) = 0;
		INVISION_API virtual void ActivateDepthTesting(MSAAMode msaa) = 0;

		INVISION_API virtual ~IGraphicsInstance() = default;

		INVISION_API IGraphicsEngine* GetEngine();
	};
}

#endif // IGRAPHICS_INSTANCE_H