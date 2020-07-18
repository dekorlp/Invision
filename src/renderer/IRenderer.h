#ifndef IRENDERER_H
#define IRENDERER_H


namespace Invision
{

	class IGraphicsEngine;
	class ICommandBuffer;

	class IRenderer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IRenderer() = delete;
		INVISION_API IRenderer(IGraphicsEngine* engine);
		INVISION_API virtual ~IRenderer() = default;

		INVISION_API IGraphicsEngine* engine();

		INVISION_API virtual bool PrepareFrame() = 0;
		INVISION_API virtual bool SubmitFrame(std::shared_ptr<ICommandBuffer> commandBuffer) = 0;
	};
}

#endif // IRENDERER_H