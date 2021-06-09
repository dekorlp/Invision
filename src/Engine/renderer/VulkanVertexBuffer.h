#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H


#include "IVertexBuffer.h"
#include "renderer\Vulkan\VulkanBaseVertexBuffer.h"

namespace Invision
{
	class VulkanInstance;

	class VulkanAttributeDescription : public IAttributeDescription
	{
	public:
		INVISION_API VulkanAttributeDescription() = delete;
		//INVISION_API VulkanAttributeDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, VulkanBaseBindingDescription& baseBindingDesc, uint32_t location, GfxFormat format, uint32_t offset);
		INVISION_API VulkanAttributeDescription(VulkanInstance* instance, VulkanBaseVertexBinding &baseBindingDescriptions, VulkanBaseBindingDescription& baseBindingDesc, uint32_t location, GfxFormat format, uint32_t offset);

	private:
		Invision::VulkanInstance *mVulkanInstance;
	};

	class VulkanBindingDescription : public IBindingDescription
	{
	public:

		INVISION_API VulkanBindingDescription();
		INVISION_API VulkanBindingDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, uint32_t binding, uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate);
		INVISION_API VulkanBindingDescription(VulkanBaseVertexBinding &vertexBinding, uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate);
		INVISION_API IBindingDescription& CreateAttribute(uint32_t location, GfxFormat format, uint32_t offset);

	private:
		Invision::VulkanInstance *mVulkanInstance;
		VulkanBaseVertexBuffer *mBaseVertexBuffer;
		VulkanBaseVertexBinding *mBaseBindingDescription;
		VulkanBaseBindingDescription mBaseBindingDesc;
		std::vector<VulkanAttributeDescription> mAttributeDesc;
	};

	class VulkanVertexBindingDescription : public IVertexBindingDescription
	{
	public:
		INVISION_API VulkanVertexBindingDescription() = default;
		//INVISION_API virtual IVertexBuffer& CreateVertexBuffer() = 0;
		INVISION_API std::shared_ptr < IBindingDescription> CreateVertexBinding(uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate) override;

		INVISION_API VulkanBaseVertexBinding  GetBaseDescription();

	private:
		VulkanBaseVertexBinding mVertexBinding;
	};

	class VulkanVertexBuffer : public IVertexBuffer
	{
	public:
		INVISION_API VulkanVertexBuffer() = delete;
		INVISION_API VulkanVertexBuffer(VulkanInstance* instance);
		
		std::shared_ptr < IBindingDescription> CreateVertexBinding(uint32_t binding, uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate);
		INVISION_API void CreateBuffer(const void *source, uint64_t size, uint32_t binding, std::shared_ptr<IVertexBindingDescription> bindingDescription) override;

		INVISION_API VulkanBaseVertexBuffer  GetBaseVertexBuffer();

		INVISION_API ~VulkanVertexBuffer();

	private:
		Invision::VulkanInstance *mVulkanInstance;

		VulkanBaseVertexBuffer mBaseVertexBuffer;
	};

}

#endif // VULKAN_VERTEX_BUFFER_H