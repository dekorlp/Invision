#ifndef IFRAMEBUFFER_H
#define IFRAMEBUFFER_H


namespace Invision
{

	class IGraphicsEngine;
	class IGraphicsInstance;

	class IRenderPass;

	class IFramebuffer
	{
		IGraphicsEngine* Engine = nullptr;
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IFramebuffer() = delete;
		INVISION_API IFramebuffer(IGraphicsInstance* instance, std::shared_ptr<IRenderPass> renderPass);
		INVISION_API virtual ~IFramebuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IFRAMEBUFFER_H