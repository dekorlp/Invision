#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"


#include "VulkanUniformBuffer.h"

namespace Invision
{
	VulkanSetLayout VulkanUniformBuffer::CreateUniformBufferSet(uint32 setNumber)
	{
		VulkanSetLayout setLayout(setNumber);
		
		this->UniformSets.push_back(setLayout);

		return setLayout;
	}

	void VulkanUniformBuffer::DestroyUniformBuffer()
	{
		for (int i = 0; i < UniformSets.size(); i++)
		{
			UniformSets[i].DestroyVulkanSetLayout();
		}
		UniformSets.clear();
	}
}