#ifndef IVERTEXBUFFER_H
#define IVERTEXBUFFER_H


namespace Invision
{

	class IGraphicsEngine;

	enum VertexFormat
	{
		FORMAT_UNEDFINED = 0,
		FORMAT_R32G32_SFLOAT = 1,
		FORMAT_R32G32B32_SFLOAT = 2,
	};

	enum VertexInputRate
	{
		VERTEX_INPUT_RATE_VERTEX = 0,
		VERTEX_INPUT_RATE_INSTANCE = 1
	};


	class IVertexBuffer
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IVertexBuffer() = delete;
		INVISION_API IVertexBuffer(IGraphicsEngine* engine);
		INVISION_API virtual ~IVertexBuffer() = default;

		INVISION_API virtual IVertexBuffer& CreateVertexBuffer(uint64_t size, const void* source, uint64_t offset) = 0;
		INVISION_API virtual IVertexBuffer& CreateVertexInput(uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate) = 0;
		INVISION_API virtual IVertexBuffer& CreateAttribute(uint32_t location, VertexFormat format, uint32_t offset) = 0;

		INVISION_API IGraphicsEngine* engine();

	};
}

#endif // IRENDERPASS_H
