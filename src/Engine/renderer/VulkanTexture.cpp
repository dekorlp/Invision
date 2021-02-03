#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanTexture.h"
namespace Invision
{


	VulkanTexture::VulkanTexture(VulkanInstance* instance) :
		ITexture(instance)
	{
		vulkanInstance = instance;
	}

	void VulkanTexture::LoadTexture(unsigned char* pixels, int imageSize, int width, int height, bool generateMipMaps)
	{

		texture.CreateTextureImage(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool(), vulkanInstance->GetCoreEngine()->GetMemoryManager(), pixels, imageSize, width, height, vulkanInstance->GetDepthRessources().AreDepthRessourcesActivated(), generateMipMaps);
		texture.CreateTextureImageView(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

	void VulkanTexture::CreateTextureImageView()
	{

		texture.CreateTextureImageView(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
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


		texture.CreateTextureSampler(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), minFilterVk, magFilterVk, addressModeUVk, addressModeVVk, addressModeWVk, MipLodBias, minLod);
	}

	VulkanBaseTexture VulkanTexture::GetBaseTexture()
	{
		 return texture;
	}

	VulkanTexture::~VulkanTexture()
	{
		texture.DestroyTexture(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

}