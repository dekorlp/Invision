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



	};

}

#endif // ITEXTURE_H