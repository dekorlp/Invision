#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"

#include "VulkanBaseTexture.h"

namespace Invision
{
	void VulkanBaseTexture::CreateTextureImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, unsigned char* pixels, int imageSize, int width, int height, bool useDepthRessource, bool generateMipMaps)
	{
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


		void* pStagingBuffer = memoryManager.BindToSharedMemory(vulkanInstance, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanInstance, pStagingBuffer, pixels);
		CreateImage(vulkanInstance, memoryManager, width, height, mMipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		TransitionImageLayout(vulkanInstance, commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, useDepthRessource, mMipLevels);
		memoryManager.CopyBufferToImage(vulkanInstance, commandPool, pStagingBuffer, mImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		memoryManager.Unbind(vulkanInstance, pStagingBuffer);

		GenerateMipmaps(vulkanInstance, commandPool, VK_FORMAT_R8G8B8A8_SRGB, width, height, mMipLevels);
	}

	void VulkanBaseTexture::CreateColorRessources(SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, SVulkanContext &vulkanContext)
	{
		mMemoryManager = &memoryManager;
		VkFormat colorFormat = vulkanContext.swapChainImageFormat;

		mpImage = CreateImage(vulkanInstance, memoryManager, vulkanContext.swapChainExtent.width, vulkanContext.swapChainExtent.height, 1, vulkanInstance.MsaaFlagBits, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage);
		mTextureImageView = CreateImageView(vulkanInstance, mImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void VulkanBaseTexture::CreateImage(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager, int width, int height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,  VkMemoryPropertyFlags properties)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(vulkanInstance.logicalDevice, &imageInfo, nullptr, &mImage) != VK_SUCCESS) {
			throw VulkanBaseException("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanInstance.logicalDevice, mImage, &memRequirements);

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

		mpImage = mMemoryManager->BindImageToDedicatedMemory(vulkanInstance, mImage, memRequirements.size);


	}

	void* VulkanBaseTexture::CreateImage(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager, int width, int height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(vulkanInstance.logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw VulkanBaseException("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanInstance.logicalDevice, image, &memRequirements);

		/*VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice, memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw VulkanBaseException("failed to allocate image memory!");
		}

		vkBindImageMemory(vulkanInstance.logicalDevice, image, imageMemory, 0);*/

		return memoryManager.BindImageToDedicatedMemory(vulkanInstance, image, memRequirements.size);

	}

	void VulkanBaseTexture::CreateTextureImageView( SVulkanBase &vulkanInstance)
	{
		mTextureImageView = CreateImageView(vulkanInstance, mImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mMipLevels);
	}

	void VulkanBaseTexture::CreateTextureSampler(SVulkanBase &vulkanInstance, VkFilter minFilter, VkFilter magFilter, VkSamplerAddressMode addressU, VkSamplerAddressMode addressV, VkSamplerAddressMode addressW, float MipLodBias, float minLod)
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

		if (vkCreateSampler(vulkanInstance.logicalDevice, &samplerInfo, nullptr, &mTextureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	void VulkanBaseTexture::TransitionImageLayout(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, bool useDepthRessource, uint32_t mipLevels)
	{
		VkCommandBuffer commandBuffer = VulkanBaseMemoryManager::BeginSingleTimeCommands(vulkanInstance, commandPool);

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
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
	
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (!useDepthRessource)
		{

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
			else {

				throw VulkanBaseException("unsupported layout transition!");
			}

		}
		else
		{
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
		}
		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);


		VulkanBaseMemoryManager::EndSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);
	}

	void VulkanBaseTexture::GenerateMipmaps(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkFormat imageFormat, int width, int height, uint32_t mipLevels)
	{

		// Check if image format supports linear blitting
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(vulkanInstance.physicalDeviceStruct.physicalDevice, imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
			throw VulkanBaseException("texture image format does not support linear blitting!");
		}

		VkCommandBuffer commandBuffer = VulkanBaseMemoryManager::BeginSingleTimeCommands(vulkanInstance, commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = mImage;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
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
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
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



		VulkanBaseMemoryManager::EndSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);

	}

	VkImageView VulkanBaseTexture::GetImageView()
	{
		return this->mTextureImageView;
	}

	VkSampler VulkanBaseTexture::GetImageSampler()
	{
		return this->mTextureSampler;
	}

	void VulkanBaseTexture::DestroyTexture(const SVulkanBase &vulkanInstance)
	{
		if (mTextureSampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(vulkanInstance.logicalDevice, mTextureSampler, nullptr);
		}

		vkDestroyImageView(vulkanInstance.logicalDevice, mTextureImageView, nullptr);
		vkDestroyImage(vulkanInstance.logicalDevice, mImage, nullptr);
		mMemoryManager->Unbind(vulkanInstance, mpImage);
		//vkFreeMemory(vulkanInstance.logicalDevice, mImageMemory, nullptr);
	}

}