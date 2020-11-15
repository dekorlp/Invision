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
#include "ITexture.h"

namespace Invision
{
	class IGraphicsEngine;
	struct CanvasDimensions;

	class IGraphicsInstance
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IGraphicsInstance() = delete;
		INVISION_API IGraphicsInstance(IGraphicsEngine* engine);
		INVISION_API IGraphicsInstance(IGraphicsEngine* engine, CanvasDimensions* canvas);
		INVISION_API virtual void ResetPresentation(CanvasDimensions canvas) = 0;

		INVISION_API virtual std::shared_ptr<IRenderer> CreateRenderer() = 0;
		INVISION_API virtual std::shared_ptr<IRenderPass> CreateRenderPass() = 0;
		INVISION_API virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer() = 0;
		INVISION_API virtual std::shared_ptr<IUniformBuffer> CreateUniformBuffer() = 0;
		INVISION_API virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer() = 0;
		INVISION_API virtual std::shared_ptr<IPipeline> CreatePipeline() = 0;
		INVISION_API virtual std::shared_ptr<IFramebuffer> CreateFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass) = 0;
		INVISION_API virtual std::shared_ptr<ICommandBuffer> CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer) = 0;
		INVISION_API virtual std::shared_ptr<ITexture> CreateTexture() = 0;

		INVISION_API virtual ~IGraphicsInstance() = default;

		INVISION_API IGraphicsEngine* engine();
	};
}

#endif // IGRAPHICS_INSTANCE_H