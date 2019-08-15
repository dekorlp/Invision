#ifndef VULKAN_H
#define VULKAN_H

#include "vulkan\vulkan.h"
#include "VulkanException.h"
namespace Invision
{
	class INVISION_API Vulkan
	{
	public:
		void Init();
	private:
		void CreateInstance();
		void CheckExtensions(std::vector<const char*> &requiredExtensions);
		VkInstance mVkInstance;
	};
}

#endif //VULKAN_H