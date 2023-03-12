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
		INVISION_API VulkanTexture(VulkanInstance* instance, unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps);

		INVISION_API void CreateTexture(unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps);
		INVISION_API void CreateTextureCubemap(unsigned char* posx, unsigned char* negx, unsigned char* posy, unsigned char* negy, unsigned char* posz, unsigned char* negz, int width, int height, GfxFormat format, bool generateMipMaps);
		INVISION_API void CreateTextureArray(std::vector<unsigned char*> textureArray, int width, int height, GfxFormat format, bool generateMipMaps);
		INVISION_API void CreateColorAttachment(int width, int height, GfxFormat format) override;
		INVISION_API void CreateDepthAttachment(int width, int height, bool isDepthStencil = false) override;
		INVISION_API void CreateTextureImageView(GfxFormat format);
		INVISION_API void CreateTextureSampler(SamplerFilterMode minFilter, SamplerFilterMode magFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, float MipLodBias = 0.0f, float minLod = 0.0f);

		INVISION_API VulkanBaseTexture GetBaseTexture();
		INVISION_API VkImageLayout GetImageLayout();
		INVISION_API void SetBaseTexture(VulkanBaseTexture baseTexure);

		~VulkanTexture();


	private:
		Invision::VulkanInstance *mVulkanInstance;
		Invision::VulkanBaseTexture mTexture;
		VkImageLayout mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	};

}

#endif //