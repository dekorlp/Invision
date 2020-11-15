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

	void VulkanTexture::LoadTexture(unsigned char* pixels, int imageSize, int width, int height)
	{
		texture.CreateTextureImage(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetCoreEngine()->GetCommandPool(), pixels, imageSize, width, height, 0);
	}

	void VulkanTexture::CreateTextureImageView()
	{

		texture.CreateTextureImageView(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

	void VulkanTexture::CreateTextureSampler()
	{
		texture.CreateTextureSampler(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

	VulkanTexture::~VulkanTexture()
	{
		texture.DestroyTexture(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

}