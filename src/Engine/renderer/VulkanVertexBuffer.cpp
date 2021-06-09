#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanVertexBuffer.h"
namespace Invision
{
	IBindingDescription& VulkanBindingDescription::CreateAttribute(uint32_t location, GfxFormat format, uint32_t offset)
	{
		mAttributeDesc.push_back(VulkanAttributeDescription(mVulkanInstance, *mBaseBindingDescription, mBaseBindingDesc, location, format, offset));

		return *this;
	}

	VulkanAttributeDescription::VulkanAttributeDescription(VulkanInstance* instance, VulkanBaseVertexBinding &baseBindingDescriptions, VulkanBaseBindingDescription& baseBindingDesc, uint32_t location, GfxFormat format, uint32_t offset) : IAttributeDescription(instance)
	{
		baseBindingDesc.CreateAttributeDescription(baseBindingDescriptions.GetAttributeDescriptions(), baseBindingDesc.GetBindingDescription().binding, location, instance->ConvertInvisionFormatToVkFormat(format), offset);
	}

	

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanInstance* instance) : IVertexBuffer(instance)
	{
		mVulkanInstance = instance;
	}

	VulkanBaseVertexBuffer  VulkanVertexBuffer::GetBaseVertexBuffer()
	{
		return mBaseVertexBuffer;
	}

	void VulkanVertexBuffer::CreateBuffer(const void *source, uint64_t size, uint32_t binding, std::shared_ptr<IVertexBindingDescription> bindingDescription)
	{
		mBaseVertexBuffer.CreateBuffer(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetCoreEngine()->GetCommandPool(), mVulkanInstance->GetCoreEngine()->GetMemoryManager(), binding, size, source);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		mBaseVertexBuffer.DestroyVertexBuffers(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}
	

	std::shared_ptr < IBindingDescription> VulkanVertexBindingDescription::CreateVertexBinding(uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate)
	{
		std::shared_ptr<VulkanBindingDescription> desc = std::make_shared<VulkanBindingDescription>(mVertexBinding, binding, stride, vertexInputRate);

		return desc;
	}

	VulkanBaseVertexBinding  VulkanVertexBindingDescription::GetBaseDescription()
	{
		return mVertexBinding;
	}

	VulkanBindingDescription::VulkanBindingDescription()
	{

	}


	VulkanBindingDescription::VulkanBindingDescription(VulkanBaseVertexBinding &vertexBinding, uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate)
	{
		VkVertexInputRate inputRate;

		if (vertexInputRate == VERTEX_INPUT_RATE_VERTEX)
		{
			inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}
		else if (vertexInputRate == VERTEX_INPUT_RATE_INSTANCE)
		{
			inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		}
		else
		{
			throw InvisionBaseRendererException("Unknown VertexInputRate passed to Function CreateVertexInput");
		}
		mBaseBindingDesc = vertexBinding.CreateBinding(binding, stride, inputRate);
		mBaseBindingDescription = &vertexBinding;
	}
}