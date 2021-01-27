#ifndef IRENDERER_H
#define IRENDERER_H


namespace Invision
{
	class IGraphicsInstance;
	class ICommandBuffer;

	class IRenderer
	{
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IRenderer() = delete;
		INVISION_API IRenderer(IGraphicsInstance* instance);
		INVISION_API virtual ~IRenderer() = default;

		INVISION_API virtual bool PrepareFrame() = 0;
		INVISION_API virtual void Draw(std::shared_ptr<ICommandBuffer> commandBuffer) = 0;
		INVISION_API virtual bool SubmitFrame() = 0;
	};
}

#endif // IRENDERER_H