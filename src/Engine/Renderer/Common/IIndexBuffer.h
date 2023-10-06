#ifndef IINDEX_BUFFER_H
#define IINDEX_BUFFER_H


namespace Invision
{
	class IGraphicsInstance;

	class IIndexBuffer
	{
		IGraphicsInstance* mInstance = nullptr;

	public:
		INVISION_API IIndexBuffer() = delete;
		INVISION_API IIndexBuffer(IGraphicsInstance* instance);
		INVISION_API virtual void CreateIndexBuffer(uint64_t size, const void* source) = 0;
		INVISION_API virtual ~IIndexBuffer() = default;
	};
}

#endif // IINDEX_BUFFER_H
