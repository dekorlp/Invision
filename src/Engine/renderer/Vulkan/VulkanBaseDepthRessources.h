#ifndef VULKAN_BASE_DEPTH_RESSOURCES_H
#define VULKAN_BASE_DEPTH_RESSOURCES_H

#include "vulkan\vulkan.h"

#include "VulkanBase.h"
#include "VulkanBaseTexture.h"

namespace Invision
{
	class VulkanBaseMemoryManager;
	class VulkanBaseCommandPool;

	class VulkanBaseDepthRessources : VulkanBaseTexture
	{
	private:
		VulkanBaseMemoryManager *mMemoryManager;

		bool mUseDepthRessources;

		VkImage mDepthImage;
		void* mpImage;

		VkImageView mDepthImageView;
		VkImageAspectFlags mAspectFlags;

		VkFormat FindSupportedFormat(const SVulkanBase &vulkanInstance, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	
		bool HasStencilComponent(VkFormat format);

	public:
		VulkanBaseDepthRessources();

		VkFormat FindDepthFormat(const SVulkanBase &vulkanInstance);
		void CreateDepthRessources(SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, SVulkanContext &vulkanContext);
		bool AreDepthRessourcesActivated();
		VkImageView GetDepthImageView();

		void DestroyDepthRessources(const SVulkanBase &vulkanInstance);



		
	};
}


#endif // VULKAN_BASE_DEPTH_RESSOURCES_H