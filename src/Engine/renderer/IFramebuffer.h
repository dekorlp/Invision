#ifndef IFRAMEBUFFER_H
#define IFRAMEBUFFER_H


namespace Invision
{
	class IGraphicsInstance;

	class IRenderPass;

	class IFramebuffer
	{
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IFramebuffer() = delete;
		INVISION_API IFramebuffer(IGraphicsInstance* instance, std::shared_ptr<IRenderPass> renderPass);
		INVISION_API virtual ~IFramebuffer() = default;
	};
}

#endif // IFRAMEBUFFER_H