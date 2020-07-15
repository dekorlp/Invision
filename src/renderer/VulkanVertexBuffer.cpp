#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanVertexBuffer.h"
namespace Invision
{

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanEngine* engine) :
		IVertexBuffer(engine)
	{
		this->vulkanEngine = engine;
	}


	VulkanVertexBuffer& VulkanVertexBuffer::CreateVertexBuffer(uint64_t size, const void* source, uint64_t offset)
	{
		vertexBuffer.CreateVertexBuffer(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetCommandPool(), size, source, offset);
		return *this;
	}

	VulkanVertexBuffer& VulkanVertexBuffer::CreateVertexInput(uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate)
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

		vertexBuffer.CreateVertexInputDescription(binding, stride, inputRate);
		return *this;
	}

	VulkanVertexBuffer& VulkanVertexBuffer::CreateAttribute(uint32_t location, VertexFormat format, uint32_t offset)
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


		vertexBuffer.CreateAttributeDescription(location, vkFormat, offset);
		return *this;
	}


	VulkanBaseVertexBuffer VulkanVertexBuffer::GetBuffer()
	{
		return vertexBuffer;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vertexBuffer.DestroyVertexBuffer(vulkanEngine->GetVulkanInstance());
	}
}