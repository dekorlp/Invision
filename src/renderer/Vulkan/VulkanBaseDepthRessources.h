#ifndef VULKAN_BASE_DEPTH_RESSOURCES_H
#define VULKAN_BASE_DEPTH_RESSOURCES_H

#include "vulkan\vulkan.h"
#include "VulkanBase.h"
#include "VulkanBaseTexture.h"

namespace Invision
{
	class VulkanBaseDepthRessources : VulkanBaseTexture
	{
	private:
		bool mUseDepthRessources;

		VkImage mDepthImage;
		VkDeviceMemory mDepthImageMemory;
		VkImageView mDepthImageView;
		VkImageAspectFlags mAspectFlags;

		VkFormat findSupportedFormat(const SVulkanBase &vulkanInstance, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	
		bool hasStencilComponent(VkFormat format);

	public:
		VkFormat findDepthFormat(const SVulkanBase &vulkanInstance);
		void CreateDepthRessources(SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, SVulkanContext &vulkanContext);
		bool AreDepthRessourcesActivated();


		
	};
}


#endif // VULKAN_BASE_DEPTH_RESSOURCES_H