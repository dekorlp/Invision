#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H


#include "ITexture.h"
#include "renderer\Vulkan\VulkanBaseTexture.h"

namespace Invision
{
	class VulkanInstance;

	class VulkanTexture : public ITexture
	{
	public:

		INVISION_API VulkanTexture() = delete;
		INVISION_API VulkanTexture(VulkanInstance* instance);

		INVISION_API void LoadTexture(unsigned char* pixels, int imageSize, int width, int height);
		INVISION_API void CreateTextureImageView();
		INVISION_API void CreateTextureSampler();

		~VulkanTexture();


	private:
		Invision::VulkanInstance *vulkanInstance;
		Invision::VulkanBaseTexture texture;
	};

}

#endif //