#ifndef IVERTEXBUFFER_H
#define IVERTEXBUFFER_H


namespace Invision
{

	class IGraphicsEngine;

	class IVertexBuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IVertexBuffer() = delete;
		INVISION_API IVertexBuffer(IGraphicsEngine* engine);
		INVISION_API virtual ~IVertexBuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IRENDERPASS_H
