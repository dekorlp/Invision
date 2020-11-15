#ifndef VULKAN_BASE_TEXTURE_H
#define VULKAN_BASE_TEXTURE_H

#include "vulkan\vulkan.h"
#include "VulkanBasePresentation.h"
#include "VulkanBaseBuffer.h"

namespace Invision
{


	class  VulkanBaseTexture
	{
		public:
			void CreateTextureImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, unsigned char* pixels, int imageSize, int width, int height, VkDeviceSize offset);
			void CreateTextureImageView(SVulkanBase &vulkanInstance);
			void CreateTextureSampler(SVulkanBase &vulkanInstance);
			void DestroyTexture(const SVulkanBase &vulkanInstance);
		private:
			void CreateImage(const SVulkanBase &vulkanInstance, int width, int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
			void TransitionImageLayout(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			void CopyBufferToImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkBuffer buffer, uint32_t width, uint32_t height);

			//VulkanBaseBuffer mTextureBuffer;
			VkDeviceSize mOffset;

			VkImage mImage;
			VkDeviceMemory mImageMemory;

			VkImageView mTextureImageView;

			VkSampler mTextureSampler;
	};


}

#endif //VULKAN_BASE_TEXTURE_H