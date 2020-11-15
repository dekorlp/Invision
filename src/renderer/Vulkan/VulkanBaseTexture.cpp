#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"


#include "VulkanBaseTexture.h"

namespace Invision
{
	void VulkanBaseTexture::CreateTextureImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, unsigned char* pixels, int imageSize, int width, int height, VkDeviceSize offset)
	{
		mOffset = offset;

		VulkanBaseBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(vulkanInstance, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE);


		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory(), 0, imageSize, 0, &data);
		memcpy(data, pixels, (size_t)imageSize);
		vkUnmapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory());

		CreateImage(vulkanInstance, width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		TransitionImageLayout(vulkanInstance, commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(vulkanInstance, commandPool, stagingBuffer.GetBuffer(), static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		TransitionImageLayout(vulkanInstance, commandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		stagingBuffer.DestroyBuffer(vulkanInstance);	
	}

	void VulkanBaseTexture::CreateImage(const SVulkanBase &vulkanInstance, int width, int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,  VkMemoryPropertyFlags properties)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(vulkanInstance.logicalDevice, &imageInfo, nullptr, &mImage) != VK_SUCCESS) {
			throw VulkanBaseException("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanInstance.logicalDevice, mImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice ,memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &mImageMemory) != VK_SUCCESS) {
			throw VulkanBaseException("failed to allocate image memory!");
		}

		vkBindImageMemory(vulkanInstance.logicalDevice, mImage, mImageMemory, 0);
	}

	void VulkanBaseTexture::CreateTextureImageView( SVulkanBase &vulkanInstance)
	{
		mTextureImageView = VulkanBasePresentation::CreateImageView(vulkanInstance, mImage, VK_FORMAT_R8G8B8A8_SRGB);
	}

	void VulkanBaseTexture::CreateTextureSampler(SVulkanBase &vulkanInstance)
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(vulkanInstance.logicalDevice, &samplerInfo, nullptr, &mTextureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	void VulkanBaseTexture::TransitionImageLayout(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = VulkanBaseBuffer::beginSingleTimeCommands(vulkanInstance, commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = mImage;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
	
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


		VulkanBaseBuffer::endSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);
	}

	void VulkanBaseTexture::CopyBufferToImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkBuffer buffer, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = VulkanBaseBuffer::beginSingleTimeCommands(vulkanInstance, commandPool);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			mImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		VulkanBaseBuffer::endSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);
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
		vkDestroySampler(vulkanInstance.logicalDevice, mTextureSampler, nullptr);
		vkDestroyImageView(vulkanInstance.logicalDevice, mTextureImageView, nullptr);

		vkDestroyImage(vulkanInstance.logicalDevice, mImage, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mImageMemory, nullptr);
	}

}