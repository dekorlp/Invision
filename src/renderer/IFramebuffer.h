#ifndef IFRAMEBUFFER_H
#define IFRAMEBUFFER_H


namespace Invision
{

	class IGraphicsEngine;
	class IRenderPass;

	class IFramebuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IFramebuffer() = delete;
		INVISION_API IFramebuffer(IGraphicsEngine* engine, std::shared_ptr<IRenderPass> renderPass);
		INVISION_API virtual ~IFramebuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IFRAMEBUFFER_H