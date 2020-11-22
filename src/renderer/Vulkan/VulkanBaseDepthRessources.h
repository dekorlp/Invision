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
		float mMinDepthBound;
		float mMaxDepthBound;

		VkImage mDepthImage;
		VkDeviceMemory mDepthImageMemory;
		VkImageView mDepthImageView;
		VkImageAspectFlags mAspectFlags;

		VkFormat findSupportedFormat(const SVulkanBase &vulkanInstance, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	
		bool hasStencilComponent(VkFormat format);

	public:
		VulkanBaseDepthRessources();

		VkFormat findDepthFormat(const SVulkanBase &vulkanInstance);
		void CreateDepthRessources(SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, SVulkanContext &vulkanContext, float minDepthBound = 0.0f, float maxDepthBound = 1.0f);
		bool AreDepthRessourcesActivated();
		VkImageView GetDepthImageView();
		float GetMinDepthBound();
		float GetMaxDepthBound();

		void DestroyDepthRessources(const SVulkanBase &vulkanInstance);



		
	};
}


#endif // VULKAN_BASE_DEPTH_RESSOURCES_H