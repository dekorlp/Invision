#include "precompiled.h"

#include "ITexture.h"

#include "IGraphicsInstance.h"


namespace Invision
{
	ITexture::ITexture(IGraphicsInstance* instance) :
		mInstance(instance)
	{

	}

	ITexture::ITexture(IGraphicsInstance* instance, unsigned char* pixels, int width, int height, bool generateMipMaps) :
		mInstance(instance)
	{

	}
}