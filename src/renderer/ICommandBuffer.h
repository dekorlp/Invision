#ifndef ICOMMAND_BUFFER_H
#define ICOMMAND_BUFFER_H


namespace Invision
{

	class IGraphicsEngine;
	class IFramebuffer;

	class ICommandBuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API ICommandBuffer() = delete;
		INVISION_API ICommandBuffer(IGraphicsEngine* engine, std::shared_ptr<Invision::IFramebuffer> framebuffer);
		INVISION_API virtual ~ICommandBuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // ICOMMAND_BUFFER_H
