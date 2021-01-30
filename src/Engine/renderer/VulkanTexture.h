#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H


#include "ITexture.h"
#include "renderer\Vulkan\VulkanBaseTexture.h"

namespace Invision
{
	class VulkanInstance;

	enum SamplerAddressMode;
	enum SamplerFilterMode;

	class VulkanTexture : public ITexture
	{
	public:

		INVISION_API VulkanTexture() = delete;
		INVISION_API VulkanTexture(VulkanInstance* instance);

		INVISION_API void LoadTexture(unsigned char* pixels, int imageSize, int width, int height, bool generateMipMaps);
		INVISION_API void CreateTextureImageView();
		INVISION_API void CreateTextureSampler(SamplerFilterMode minFilter, SamplerFilterMode magFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, float MipLodBias = 0.0f, float minLod = 0.0f);

		INVISION_API VulkanBaseTexture GetBaseTexture();

		~VulkanTexture();


	private:
		Invision::VulkanInstance *vulkanInstance;
		Invision::VulkanBaseTexture texture;
	};

}

#endif //