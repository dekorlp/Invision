#ifndef ITEXTURE_H
#define ITEXTURE_H


namespace Invision
{
	class IGraphicsInstance;
	enum SamplerAddressMode;
	enum SamplerFilterMode;
	enum GfxFormat;

	class ITexture
	{
		IGraphicsInstance* mInstance = nullptr;

		public:
		INVISION_API ITexture() = delete;
		INVISION_API ITexture(IGraphicsInstance* instance);
		INVISION_API ITexture(IGraphicsInstance* instance, unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps);
		INVISION_API virtual ~ITexture() = default;

		INVISION_API virtual void CreateTexture(unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps = false) = 0;
		INVISION_API virtual void CreateTextureCubemap(unsigned char* posx, unsigned char* negx, unsigned char* posy, unsigned char* negy, unsigned char* posz, unsigned char* negz, int width, int height, GfxFormat format, bool generateMipMaps) = 0;
		INVISION_API virtual void CreateTextureArray(std::vector<unsigned char*> textureArray, int width, int height, GfxFormat format, bool generateMipMaps) = 0;

		INVISION_API virtual void CreateColorAttachment(int width, int height, GfxFormat format) = 0;
		INVISION_API virtual void CreateDepthAttachment(int width, int height, bool isDepthStencil = false) = 0;
		INVISION_API virtual void CreateTextureSampler(SamplerFilterMode minFilter, SamplerFilterMode magFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, float MipLodBias = 0.0f, float minLod = 0.0f) = 0;

	};

}

#endif // ITEXTURE_H