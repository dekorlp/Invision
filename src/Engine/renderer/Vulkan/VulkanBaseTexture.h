#ifndef VULKAN_BASE_TEXTURE_H
#define VULKAN_BASE_TEXTURE_H

#include "vulkan\vulkan.h"
#include "VulkanBasePresentation.h"

namespace Invision
{

	class VulkanBaseMemoryManager;
	class VulkanBaseCommandPool;

	class  VulkanBaseTexture
	{
		public:
			void CreateTextureImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, unsigned char* pixels, int imageSize, int width, int height, bool useDepthRessource, bool generateMipMaps = false);
			void CreateTextureImageView(SVulkanBase &vulkanInstance);
			void CreateTextureSampler(SVulkanBase &vulkanInstance, VkFilter minFilter, VkFilter magFilter, VkSamplerAddressMode addressU, VkSamplerAddressMode addressV, VkSamplerAddressMode addressW, float MipLodBias, float minLod);
			void DestroyTexture(const SVulkanBase &vulkanInstance);

			VkImageView GetImageView();
			VkSampler GetImageSampler();

		protected:
			void* CreateImage(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager, int width, int height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image);

		private:
			void CreateImage(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager, int width, int height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
			void TransitionImageLayout(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, bool useDepthRessource, uint32_t mipLevels);
			void CopyBufferToImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkBuffer buffer, uint32_t width, uint32_t height);
			void GenerateMipmaps(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkFormat imageFormat, int width, int height, uint32_t mipLevels);

			//VulkanBaseBuffer mTextureBuffer;
			VulkanBaseMemoryManager *mMemoryManager;

			uint32_t mMipLevels;
			VkDeviceMemory mImageMemory;

			VkImage mImage;
			void* mpImage;

			VkImageView mTextureImageView;
			VkSampler mTextureSampler;
	};


}

#endif //VULKAN_BASE_TEXTURE_H