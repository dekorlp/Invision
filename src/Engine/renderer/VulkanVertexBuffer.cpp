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
	

	//VulkanAttributeDescription::VulkanAttributeDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, VulkanBaseBindingDescription& baseBindingDesc, uint32_t location, GfxFormat format, uint32_t offset) : IAttributeDescription(instance)
	//{
	//	baseBindingDesc.CreateAttributeDescription(baseVertexBuffer.GetAttributeDescriptions(), baseBindingDesc.GetBindingDescription().binding, location, instance->ConvertInvisionFormatToVkFormat(format), offset);
	//}

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

	std::shared_ptr < IBindingDescription> VulkanVertexBuffer::CreateVertexBinding(uint32_t binding, uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate)
	{
		std::shared_ptr<VulkanBindingDescription> desc = std::make_shared<VulkanBindingDescription>(mVulkanInstance, mBaseVertexBuffer, binding, size, source, stride, vertexInputRate);
		
		return desc;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		mBaseVertexBuffer.DestroyVertexBuffers(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}
	


	//INVISION_API virtual IVertexBuffer& CreateVertexBuffer() = 0;
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

	VulkanBindingDescription::VulkanBindingDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, uint32_t binding, uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate) : IBindingDescription(instance)
	{
		mVulkanInstance = instance;
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
		mBaseBindingDesc = baseVertexBuffer.CreateBinding(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetCoreEngine()->GetCommandPool(), mVulkanInstance->GetCoreEngine()->GetMemoryManager(), binding, size, source, stride, inputRate);
		mBaseVertexBuffer = &baseVertexBuffer;
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