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

	void VulkanIndexBuffer::CreateIndexBuffer(uint64_t size, const void* source)
	{
		indexBuffer.CreateIndexBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool(), vulkanInstance->GetCoreEngine()->GetMemoryManager(), size, source);
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