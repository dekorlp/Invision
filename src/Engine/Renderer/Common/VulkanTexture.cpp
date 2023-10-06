#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanTexture.h"
namespace Invision
{


	VulkanTexture::VulkanTexture(VulkanInstance* instance) :
		ITexture(instance)
	{
		mVulkanInstance = instance;
	}

	VulkanTexture::VulkanTexture(VulkanInstance* instance, unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps) :
		ITexture(instance)
	{
		mVulkanInstance = instance;

		mTexture.CreateTextureImage(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), pixels, width, height, instance->ConvertInvisionFormatToVkFormat(format), generateMipMaps);
		mTexture.CreateTextureImageView(mVulkanInstance->GetVulkanContext(), VK_IMAGE_VIEW_TYPE_2D, instance->ConvertInvisionFormatToVkFormat(format), 1);
		mImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void VulkanTexture::CreateTexture(unsigned char* pixels,  int width, int height, GfxFormat format, bool generateMipMaps)
	{

		mTexture.CreateTextureImage(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), pixels, width, height, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), generateMipMaps);
		mTexture.CreateTextureImageView(mVulkanInstance->GetVulkanContext(), VK_IMAGE_VIEW_TYPE_2D, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), 1);
		mImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void VulkanTexture::CreateTextureCubemap(unsigned char* posx, unsigned char* negx, unsigned char* posy, unsigned char* negy, unsigned char* posz, unsigned char* negz, int width, int height, GfxFormat format, bool generateMipMaps)
	{
		mTexture.CreateTextureCubemap(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), posx, negx, posy, negy, posz, negz, width, height, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), generateMipMaps);
		mTexture.CreateTextureImageView(mVulkanInstance->GetVulkanContext(), VK_IMAGE_VIEW_TYPE_CUBE, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), VK_REMAINING_ARRAY_LAYERS);
		mImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void VulkanTexture::CreateTextureArray(std::vector<unsigned char*> textureArray, int width, int height, GfxFormat format, bool generateMipMaps)
	{
		mTexture.CreateTextureArray(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), textureArray, width, height, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), generateMipMaps);
		mTexture.CreateTextureImageView(mVulkanInstance->GetVulkanContext(), VK_IMAGE_VIEW_TYPE_2D_ARRAY, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), VK_REMAINING_ARRAY_LAYERS);
		mImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void VulkanTexture::CreateColorAttachment(int width, int height, GfxFormat format)
	{
		mTexture.CreateColorRessources(mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), width, height, VK_SAMPLE_COUNT_1_BIT, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
		mImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void VulkanTexture::CreateDepthAttachment(int width, int height, bool isDepthStencil)
	{
		mTexture.CreateDepthRessources(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetCommandPool(), mVulkanInstance->GetMemoryManager(), width, height, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
		if (!isDepthStencil)
		{
			mImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
		else
		{
			mImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}
	}

	void VulkanTexture::CreateTextureImageView(GfxFormat format)
	{

		mTexture.CreateTextureImageView(mVulkanInstance->GetVulkanContext(), VK_IMAGE_VIEW_TYPE_2D, mVulkanInstance->ConvertInvisionFormatToVkFormat(format), 1);
	}

	void VulkanTexture::CreateTextureSampler(SamplerFilterMode minFilter, SamplerFilterMode magFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, float MipLodBias, float minLod)
	{
		 VkFilter minFilterVk;
		 VkFilter magFilterVk;
		 VkSamplerAddressMode addressModeUVk;
		 VkSamplerAddressMode addressModeVVk;
		 VkSamplerAddressMode addressModeWVk;

		 switch (addressU)
		 {
		 case SAMPLER_ADDRESS_MODE_REPEAT:
			 addressModeUVk = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			 break;
		 case SAMPLER_ADDRESS_MODE_CLAMP:
			 addressModeUVk = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			 break;
		 case SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
			 addressModeUVk = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			 break;
		 case VK_SAMPLER_ADDRESS_MODE_MIRRORED_ONCE:
			 addressModeUVk = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
			 break;
		 }

		 switch (addressV)
		 {
		 case SAMPLER_ADDRESS_MODE_REPEAT:
			 addressModeVVk = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			 break;
		 case SAMPLER_ADDRESS_MODE_CLAMP:
			 addressModeVVk = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			 break;
		 case SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
			 addressModeVVk = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			 break;
		 case VK_SAMPLER_ADDRESS_MODE_MIRRORED_ONCE:
			 addressModeVVk = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
			 break;
		 }

		 switch (addressW)
		 {
		 case SAMPLER_ADDRESS_MODE_REPEAT:
			 addressModeWVk = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			 break;
		 case SAMPLER_ADDRESS_MODE_CLAMP:
			 addressModeWVk = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			 break;
		 case SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
			 addressModeWVk = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			 break;
		 case VK_SAMPLER_ADDRESS_MODE_MIRRORED_ONCE:
			 addressModeWVk = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
			 break;
		 }

		 switch (minFilter)
		 {
		 case SAMPLER_FILTER_MODE_LINEAR:
			 minFilterVk = VK_FILTER_LINEAR;
			 break;
		 case SAMPLER_FILTER_MODE_NEAREST:
			 minFilterVk = VK_FILTER_NEAREST;
			 break;
		 }

		 switch (magFilter)
		 {
		 case SAMPLER_FILTER_MODE_LINEAR:
			 magFilterVk = VK_FILTER_LINEAR;
			 break;
		 case SAMPLER_FILTER_MODE_NEAREST:
			 magFilterVk = VK_FILTER_NEAREST;
			 break;
		 }


		mTexture.CreateTextureSampler(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(), minFilterVk, magFilterVk, addressModeUVk, addressModeVVk, addressModeWVk, MipLodBias, minLod);
	}

	VulkanBaseTexture VulkanTexture::GetBaseTexture()
	{
		 return mTexture;
	}

	void VulkanTexture::SetBaseTexture(VulkanBaseTexture baseTexure)
	{
		mTexture = baseTexure;
	}

	VkImageLayout VulkanTexture::GetImageLayout()
	{
		return mImageLayout;
	}

	VulkanTexture::~VulkanTexture()
	{
		mTexture.DestroyTexture(mVulkanInstance->GetVulkanContext());
	}

}