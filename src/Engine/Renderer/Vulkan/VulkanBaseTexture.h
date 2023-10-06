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
			void CreateTextureImage(const SVulkanBase& vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, unsigned char* pixels, int width, int height, VkFormat format, bool generateMipMaps = false);
			void CreateTextureCubemap(const SVulkanBase& vulkanInstance, const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, unsigned char* posx, unsigned char* negx, unsigned char* posy, unsigned char* negy, unsigned char* posz, unsigned char* negz, int width, int height, VkFormat format, bool generateMipMaps = false);
			void CreateTextureArray(const SVulkanBase& vulkanInstance, const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, std::vector<unsigned char*> textureArray, int width, int height, VkFormat format, bool generateMipMaps = false);
			void CreateColorRessources(SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, int width, int height, VkSampleCountFlagBits sampleCountFlags, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags aspectFlags);
			void CreateDepthRessources(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, int width, int height, VkSampleCountFlagBits sampleCountFlags, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags aspectFlags);

			VkFormat FindDepthFormat(const SVulkanBase &vulkanInstance);

			void CreateTextureImageView(SVulkanContext& vulkanContext, VkImageViewType viewType, VkFormat format, uint32_t layerCount);
			void CreateTextureSampler(SVulkanBase &vulkanInstance, SVulkanContext& vulkanContext, VkFilter minFilter, VkFilter magFilter, VkSamplerAddressMode addressU, VkSamplerAddressMode addressV, VkSamplerAddressMode addressW, float MipLodBias, float minLod);
			void DestroyTexture(const SVulkanContext &vulkanContext);



			VkImageView GetImageView();
			VkSampler GetImageSampler();
			VkFormat GetFormat();

		protected:
			void* CreateImage(const SVulkanContext& vulkanContext, VulkanBaseMemoryManager& memoryManager, int width, int height, bool isCubemap, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image);

		private:
			void CreateImage(const SVulkanContext& vulkanContext, VulkanBaseMemoryManager& memoryManager, int width, int height, bool isCubemap, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
			void TransitionImageLayout(const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, uint32_t baseArrayLayer, uint32_t layerCount);
			//void CopyBufferToImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkBuffer buffer, uint32_t width, uint32_t height);
			void GenerateMipmaps(const SVulkanBase &vulkanInstance, const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VkFormat imageFormat, int width, int height, uint32_t mipLevels, uint32_t baseArrayLayer);

			// Depth Texture Methods
			VkFormat FindSupportedFormat(const SVulkanBase &vulkanInstance, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			
			
			bool HasStencilComponent(VkFormat format);

			//VulkanBaseBuffer mTextureBuffer;
			VulkanBaseMemoryManager *mMemoryManager;

			uint32_t mMipLevels;
			VkDeviceMemory mImageMemory;
			
			VkFormat mFormat = VK_FORMAT_UNDEFINED;
			VkImage mImage = VK_NULL_HANDLE;
			void* mpImage;

			VkImageView mTextureImageView = VK_NULL_HANDLE;
			VkSampler mTextureSampler = VK_NULL_HANDLE;
	};


}

#endif //VULKAN_BASE_TEXTURE_H