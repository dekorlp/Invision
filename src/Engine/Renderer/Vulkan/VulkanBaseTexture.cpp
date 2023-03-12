#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"

#include "VulkanBaseTexture.h"

namespace Invision
{
	void VulkanBaseTexture::CreateTextureImage(const SVulkanBase& vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, unsigned char* pixels, int width, int height, VkFormat format, bool generateMipMaps)
	{
		mFormat = format;
		int imageSize = width * height * 4; //4 four channels for RGBA -> Color Formats of RGB are motly not supported by modern GPU Devices

		mMemoryManager = &memoryManager;

		// mip map Generation
		if (generateMipMaps)
		{
			mMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		}
		else
		{
			mMipLevels = 1;
		}

		CreateImage(vulkanContext, memoryManager, width, height, false, mMipLevels, 1, VK_SAMPLE_COUNT_1_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		void* pStagingBuffer = memoryManager.BindToSharedMemory(vulkanContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanContext, pStagingBuffer, pixels);
		TransitionImageLayout(vulkanContext, commandPool, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mMipLevels, 0, 1);
		memoryManager.CopyBufferToImage(vulkanContext, commandPool, pStagingBuffer, mImage, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		memoryManager.Unbind(vulkanContext, pStagingBuffer);

		GenerateMipmaps(vulkanInstance, vulkanContext, commandPool, format, width, height, mMipLevels, 0);
	}

	void VulkanBaseTexture::CreateTextureCubemap(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, unsigned char* posx, unsigned char* negx, unsigned char* posy, unsigned char* negy, unsigned char* posz, unsigned char* negz, int width, int height, VkFormat format, bool generateMipMaps )
	{
		std::vector<unsigned char*> cubemap_images = {
			posx, negx, posy, negy, posz, negz
		};


		mFormat = format;
		int imageSize = width * height * 4; //4 four channels for RGBA -> Color Formats of RGB are motly not supported by modern GPU Devices

		mMemoryManager = &memoryManager;

		// mip map Generation
		if (generateMipMaps)
		{
			mMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		}
		else
		{
			mMipLevels = 1;
		}

		CreateImage(vulkanContext, memoryManager, width, height, true, mMipLevels, 1, VK_SAMPLE_COUNT_1_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		for (unsigned int i = 0; i < 6; i++)
		{
			void* pStagingBuffer = memoryManager.BindToSharedMemory(vulkanContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
			memoryManager.CopyDataToBuffer(vulkanContext, pStagingBuffer, cubemap_images[i]);
			TransitionImageLayout(vulkanContext, commandPool, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mMipLevels, i, 1);
			memoryManager.CopyBufferToImage(vulkanContext, commandPool, pStagingBuffer, mImage, i, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
			memoryManager.Unbind(vulkanContext, pStagingBuffer);
			GenerateMipmaps(vulkanInstance, vulkanContext, commandPool, format, width, height, mMipLevels, i);
		}
	}

	void VulkanBaseTexture::CreateTextureArray(const SVulkanBase& vulkanInstance, const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, std::vector<unsigned char*> textureArray, int width, int height, VkFormat format, bool generateMipMaps)
	{

		mFormat = format;
		int imageSize = width * height * 4; //4 four channels for RGBA -> Color Formats of RGB are motly not supported by modern GPU Devices

		mMemoryManager = &memoryManager;

		// mip map Generation
		if (generateMipMaps)
		{
			mMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		}
		else
		{
			mMipLevels = 1;
		}

		CreateImage(vulkanContext, memoryManager, width, height, false, mMipLevels, 1, VK_SAMPLE_COUNT_1_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		for (unsigned int i = 0; i < textureArray.size(); i++)
		{
			void* pStagingBuffer = memoryManager.BindToSharedMemory(vulkanContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
			memoryManager.CopyDataToBuffer(vulkanContext, pStagingBuffer, textureArray[i]);
			TransitionImageLayout(vulkanContext, commandPool, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mMipLevels, i, 1);
			memoryManager.CopyBufferToImage(vulkanContext, commandPool, pStagingBuffer, mImage, i, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
			memoryManager.Unbind(vulkanContext, pStagingBuffer);
			GenerateMipmaps(vulkanInstance, vulkanContext, commandPool, format, width, height, mMipLevels, i);
		}
	}

	void VulkanBaseTexture::CreateColorRessources(SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, int width, int height, VkSampleCountFlagBits sampleCountFlags, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags aspectFlags)
	{
		mMemoryManager = &memoryManager;
		mFormat = format;
		//VkFormat colorFormat = vulkanContext.swapChainImageFormat;

		mpImage = CreateImage(vulkanContext, memoryManager, width, height, false, 1, 1, sampleCountFlags, format, tiling, usage, memoryPropertyFlags, mImage);
		mTextureImageView = CreateImageView(vulkanContext, mImage, VK_IMAGE_VIEW_TYPE_2D, format, aspectFlags, 1, 1);
	}

	void VulkanBaseTexture::CreateDepthRessources(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, int width, int height, VkSampleCountFlagBits sampleCountFlags, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags aspectFlags)
	{
		mMemoryManager = &memoryManager;
		VkFormat depthFormat = FindDepthFormat(vulkanInstance);
		mFormat = depthFormat;
		mpImage = CreateImage(vulkanContext, memoryManager, width, height, false, 1, 1, sampleCountFlags, depthFormat, tiling, usage, memoryPropertyFlags, mImage);
		mTextureImageView = CreateImageView(vulkanContext, mImage, VK_IMAGE_VIEW_TYPE_2D, depthFormat, aspectFlags, 1, 1);
	}


	void VulkanBaseTexture::CreateImage(const SVulkanContext &vulkanContext, VulkanBaseMemoryManager& memoryManager, int width, int height, bool isCubemap, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,  VkMemoryPropertyFlags properties)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = isCubemap ? 6 * arrayLayers : arrayLayers;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = isCubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u;

		if (vkCreateImage(vulkanContext.logicalDevice, &imageInfo, nullptr, &mImage) != VK_SUCCESS) {
			throw VulkanBaseException("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanContext.logicalDevice, mImage, &memRequirements);

		/*VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanInstance.logicalDevice, mImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice ,memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &mImageMemory) != VK_SUCCESS) {
			throw VulkanBaseException("failed to allocate image memory!");
		}

		vkBindImageMemory(vulkanInstance.logicalDevice, mImage, mImageMemory, 0);*/

		mpImage = mMemoryManager->BindImageToDedicatedMemory(vulkanContext, mImage, memRequirements.size);


	}

	void* VulkanBaseTexture::CreateImage(const SVulkanContext &vulkanContext, VulkanBaseMemoryManager& memoryManager, int width, int height, bool isCubemap, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = isCubemap ? 6 * arrayLayers : arrayLayers;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = isCubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u;

		if (vkCreateImage(vulkanContext.logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw VulkanBaseException("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanContext.logicalDevice, image, &memRequirements);

		/*VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice, memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw VulkanBaseException("failed to allocate image memory!");
		}

		vkBindImageMemory(vulkanInstance.logicalDevice, image, imageMemory, 0);*/

		return memoryManager.BindImageToDedicatedMemory(vulkanContext, image, memRequirements.size);

	}

	void VulkanBaseTexture::CreateTextureImageView( SVulkanContext &vulkanContext, VkImageViewType viewType, VkFormat format, uint32_t layerCount)
	{
		mTextureImageView = CreateImageView(vulkanContext, mImage, viewType, format, VK_IMAGE_ASPECT_COLOR_BIT, mMipLevels, layerCount);
	}

	void VulkanBaseTexture::CreateTextureSampler(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VkFilter minFilter, VkFilter magFilter, VkSamplerAddressMode addressU, VkSamplerAddressMode addressV, VkSamplerAddressMode addressW, float MipLodBias, float minLod)
	{
		

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = magFilter;
		samplerInfo.minFilter = minFilter;
		samplerInfo.addressModeU = addressU;
		samplerInfo.addressModeV = addressV;
		samplerInfo.addressModeW = addressW;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = vulkanInstance.physicalDeviceStruct.deviceProperties.limits.maxSamplerAnisotropy; // 16.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = MipLodBias;
		samplerInfo.minLod = minLod;
		samplerInfo.maxLod = static_cast<float>(mMipLevels);

		if (vkCreateSampler(vulkanContext.logicalDevice, &samplerInfo, nullptr, &mTextureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	void VulkanBaseTexture::TransitionImageLayout(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, uint32_t baseArrayLayer, uint32_t layerCount)
	{
		VkCommandBuffer commandBuffer = VulkanBaseMemoryManager::BeginSingleTimeCommands(vulkanContext, commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = mImage;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier.subresourceRange.layerCount = layerCount;
	
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			throw VulkanBaseException("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);


		VulkanBaseMemoryManager::EndSingleTimeCommands(vulkanContext, commandPool, commandBuffer);
	}

	void VulkanBaseTexture::GenerateMipmaps(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VkFormat imageFormat, int width, int height, uint32_t mipLevels, uint32_t baseArrayLayer)
	{

		// Check if image format supports linear blitting
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(vulkanInstance.physicalDeviceStruct.physicalDevice, imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
			throw VulkanBaseException("texture image format does not support linear blitting!");
		}

		VkCommandBuffer commandBuffer = VulkanBaseMemoryManager::BeginSingleTimeCommands(vulkanContext, commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = mImage;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = width;
		int32_t mipHeight = height;

		for (uint32_t i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = baseArrayLayer;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = baseArrayLayer;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
				mImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);



		VulkanBaseMemoryManager::EndSingleTimeCommands(vulkanContext, commandPool, commandBuffer);

	}

	VkImageView VulkanBaseTexture::GetImageView()
	{
		return this->mTextureImageView;
	}

	VkSampler VulkanBaseTexture::GetImageSampler()
	{
		return this->mTextureSampler;
	}

	VkFormat VulkanBaseTexture::GetFormat()
	{
		return mFormat;
	}

	// Depth Texture Methods

	VkFormat VulkanBaseTexture::FindSupportedFormat(const SVulkanBase &vulkanInstance, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates) {
			VkFormatProperties props;

			vkGetPhysicalDeviceFormatProperties(vulkanInstance.physicalDeviceStruct.physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw VulkanBaseException("failed to find supported format!");
	}

	VkFormat VulkanBaseTexture::FindDepthFormat(const SVulkanBase &vulkanInstance)
	{
		return FindSupportedFormat(vulkanInstance,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	bool VulkanBaseTexture::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}


	void VulkanBaseTexture::DestroyTexture(const SVulkanContext &vulkanContext)
	{
		if (mTextureSampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(vulkanContext.logicalDevice, mTextureSampler, nullptr);
		}

		vkDestroyImageView(vulkanContext.logicalDevice, mTextureImageView, nullptr);
		vkDestroyImage(vulkanContext.logicalDevice, mImage, nullptr);
		mMemoryManager->Unbind(vulkanContext, mpImage);
		//vkFreeMemory(vulkanInstance.logicalDevice, mImageMemory, nullptr);
	}

}