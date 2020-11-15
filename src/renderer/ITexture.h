#ifndef ITEXTURE_H
#define ITEXTURE_H


namespace Invision
{
	class IGraphicsInstance;


	class ITexture
	{
		IGraphicsInstance* instance = nullptr;

		public:
		INVISION_API ITexture() = delete;
		INVISION_API ITexture(IGraphicsInstance* instance);
		INVISION_API virtual ~ITexture() = default;

		INVISION_API virtual void LoadTexture(unsigned char* pixels, int imageSize, int width, int height) = 0;
		INVISION_API virtual void CreateTextureImageView() = 0;
		INVISION_API virtual void CreateTextureSampler() = 0;

	};

}

#endif // ITEXTURE_H