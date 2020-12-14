#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanVertexBuffer.h"
namespace Invision
{
	IBindingDescription& VulkanBindingDescription::CreateAttribute(uint32_t location, VertexFormat format, uint32_t offset)
	{
		mAttributeDesc.push_back(VulkanAttributeDescription(vulkanInstance, *mBaseVertexBuffer, mBaseBindingDesc, location, format, offset));

		return *this;
	}
	

	VulkanAttributeDescription::VulkanAttributeDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, VulkanBaseBindingDescription& baseBindingDesc, uint32_t location, VertexFormat format, uint32_t offset) : IAttributeDescription(instance)
	{
		VkFormat vkFormat = VK_FORMAT_UNDEFINED;

		switch (format)
		{
		case FORMAT_R32G32_SFLOAT:
			vkFormat = VK_FORMAT_R32G32_SFLOAT;
			break;
		case FORMAT_R32G32B32_SFLOAT:
			vkFormat = VK_FORMAT_R32G32B32_SFLOAT;
			break;
		default:
			vkFormat = VK_FORMAT_UNDEFINED;
		}

		baseBindingDesc.CreateAttributeDescription(baseVertexBuffer.GetAttributeDescriptions(), baseBindingDesc.GetBindingDescription().binding, location, vkFormat, offset);
	}

	

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanInstance* instance) : IVertexBuffer(instance)
	{
		vulkanInstance = instance;
	}

	VulkanBaseVertexBuffer  VulkanVertexBuffer::GetBaseVertexBuffer()
	{
		return mBaseVertexBuffer;
	}

	std::shared_ptr < IBindingDescription> VulkanVertexBuffer::CreateVertexBinding(uint64_t size, const void *source, uint64_t offset, uint32_t stride, VertexInputRate vertexInputRate)
	{
		std::shared_ptr<VulkanBindingDescription> desc = std::make_shared<VulkanBindingDescription>(vulkanInstance, mBaseVertexBuffer, size, source, offset, stride, vertexInputRate);
		
		return desc;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		mBaseVertexBuffer.DestroyVertexBuffers(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
	
	VulkanBindingDescription::VulkanBindingDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, uint64_t size, const void *source, uint64_t offset, uint32_t stride, VertexInputRate vertexInputRate) : IBindingDescription(instance)
	{
		vulkanInstance = instance;
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
		mBaseBindingDesc = baseVertexBuffer.CreateBinding(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool(), size, source, offset, stride, inputRate);
		mBaseVertexBuffer = &baseVertexBuffer;
	}
}