#ifndef IINDEX_BUFFER_H
#define IINDEX_BUFFER_H


namespace Invision
{

	class IGraphicsEngine;

	class IIndexBuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IIndexBuffer() = delete;
		INVISION_API IIndexBuffer(IGraphicsEngine* engine);
		INVISION_API virtual void CreateIndexBuffer(uint64_t size, const void* source, uint64_t offset) = 0;
		INVISION_API virtual ~IIndexBuffer() = default;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IINDEX_BUFFER_H
