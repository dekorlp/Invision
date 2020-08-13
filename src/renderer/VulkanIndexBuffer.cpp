#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanIndexBuffer.h"
namespace Invision
{

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanInstance* instance) :
		IIndexBuffer(instance)
	{
		vulkanInstance = instance;
	}

	void VulkanIndexBuffer::CreateIndexBuffer(uint64_t size, const void* source, uint64_t offset)
	{
		indexBuffer.CreateIndexBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool(), size, source, offset);
	}

	VulkanBaseIndexBuffer VulkanIndexBuffer::GetBuffer()
	{
		return indexBuffer;
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		indexBuffer.DestroyIndexBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
}