#ifndef IUNIFORM_BUFFER_H
#define IUNIFORM_BUFFER_H


namespace Invision
{

	class IGraphicsEngine;

	class IUniformBuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IUniformBuffer() = delete;
		INVISION_API IUniformBuffer(IGraphicsEngine* engine);
		INVISION_API virtual ~IUniformBuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IUNIFORM_BUFFER_H
