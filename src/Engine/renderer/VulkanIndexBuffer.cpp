#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanIndexBuffer.h"
namespace Invision
{

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanInstance* instance) :
		IIndexBuffer(instance)
	{
		mVulkanInstance = instance;
	}

	void VulkanIndexBuffer::CreateIndexBuffer(uint64_t size, const void* source)
	{
		mIndexBuffer.CreateIndexBuffer(mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), size, source);
	}

	VulkanBaseIndexBuffer VulkanIndexBuffer::GetBuffer()
	{
		return mIndexBuffer;
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		mIndexBuffer.DestroyIndexBuffer(mVulkanInstance->GetVulkanContext());
	}
}