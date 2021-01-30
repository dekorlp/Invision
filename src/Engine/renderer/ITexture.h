#ifndef ITEXTURE_H
#define ITEXTURE_H


namespace Invision
{
	class IGraphicsInstance;
	enum SamplerAddressMode;
	enum SamplerFilterMode;

	class ITexture
	{
		IGraphicsInstance* instance = nullptr;

		public:
		INVISION_API ITexture() = delete;
		INVISION_API ITexture(IGraphicsInstance* instance);
		INVISION_API virtual ~ITexture() = default;

		INVISION_API virtual void LoadTexture(unsigned char* pixels, int imageSize, int width, int height, bool generateMipMaps = false) = 0;
		INVISION_API virtual void CreateTextureImageView() = 0;
		INVISION_API virtual void CreateTextureSampler(SamplerFilterMode minFilter, SamplerFilterMode magFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, float MipLodBias = 0.0f, float minLod = 0.0f) = 0;

	};

}

#endif // ITEXTURE_H