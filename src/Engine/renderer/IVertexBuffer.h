#ifndef IVERTEXBUFFER_H
#define IVERTEXBUFFER_H


namespace Invision
{
	class IGraphicsInstance;
	class IVertexBuffer;
	class IBindingDescription;
	class IAttributeDescription;

	// used enumerations
	enum GfxFormat;
	enum VertexInputRate;


	
	class IBindingDescription
	{
		IGraphicsInstance* mInstance = nullptr;
	public:

		INVISION_API IBindingDescription() = delete;
		INVISION_API IBindingDescription(IGraphicsInstance* instance);
		INVISION_API virtual IBindingDescription& CreateAttribute(uint32_t location, GfxFormat format, uint32_t offset) = 0;
	};
	
	class IAttributeDescription
	{
		IGraphicsInstance* mInstance = nullptr;
	
		public:
			INVISION_API IAttributeDescription() = delete;
			INVISION_API IAttributeDescription(IGraphicsInstance* instance);
	};
	
	class IVertexBuffer
	{
		IGraphicsInstance* mInstance = nullptr;

	public:
		INVISION_API IVertexBuffer() = delete;
		INVISION_API IVertexBuffer(IGraphicsInstance* instance);
		//INVISION_API virtual IVertexBuffer& CreateVertexBuffer() = 0;
		INVISION_API virtual std::shared_ptr < IBindingDescription> CreateVertexBinding(uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate) = 0;
		INVISION_API virtual ~IVertexBuffer() = default;
	};
}

#endif // IRENDERPASS_H
