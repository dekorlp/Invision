#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanIndexBuffer.h"
namespace Invision
{

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanEngine* engine) :
		IIndexBuffer(engine)
	{
		vulkanEngine = engine;
	}

	void VulkanIndexBuffer::CreateIndexBuffer(uint64_t size, const void* source, uint64_t offset)
	{
		indexBuffer.CreateIndexBuffer(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetCommandPool(), size, source, offset);
	}

	VulkanBaseIndexBuffer VulkanIndexBuffer::GetBuffer()
	{
		return indexBuffer;
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		indexBuffer.DestroyIndexBuffer(vulkanEngine->GetVulkanInstance());
	}
}