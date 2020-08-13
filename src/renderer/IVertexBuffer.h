#ifndef IVERTEXBUFFER_H
#define IVERTEXBUFFER_H


namespace Invision
{

	class IGraphicsEngine;
	class IGraphicsInstance;

	// used enumerations
	enum VertexFormat;
	enum VertexInputRate;


	class IVertexBuffer
	{
		IGraphicsEngine* Engine = nullptr;
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IVertexBuffer() = delete;
		INVISION_API IVertexBuffer(IGraphicsInstance* instance);
		INVISION_API virtual ~IVertexBuffer() = default;

		INVISION_API virtual IVertexBuffer& CreateVertexBuffer(uint64_t size, const void* source, uint64_t offset) = 0;
		INVISION_API virtual IVertexBuffer& CreateVertexInput(uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate) = 0;
		INVISION_API virtual IVertexBuffer& CreateAttribute(uint32_t location, VertexFormat format, uint32_t offset) = 0;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IRENDERPASS_H
