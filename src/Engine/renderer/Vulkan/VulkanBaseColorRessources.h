#ifndef VULKAN_BASE_COLOR_RESSOURCES_H
#define VULKAN_BASE_COLOR_RESSOURCES_H

#include "vulkan\vulkan.h"

#include "VulkanBase.h"
#include "VulkanBaseTexture.h"

namespace Invision
{
	class VulkanBaseColorRessources : VulkanBaseTexture
	{
	private:
		VkImage mColorImage;
		VkDeviceMemory mColorImageMemory;
		VkImageView mColorImageView;
	public:

		void CreateColorRessources(SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, SVulkanContext &vulkanContext);

		void DestroyColorRessources(const SVulkanBase &vulkanInstance);

		VkImageView GetColorImageView();



		
	};
}


#endif // VULKAN_BASE_DEPTH_RESSOURCES_H