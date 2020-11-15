#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H


#include "ITexture.h"


namespace Invision
{
	class VulkanInstance;

	class VulkanTexture : public ITexture
	{
	public:

		INVISION_API VulkanTexture() = delete;

		INVISION_API VulkanTexture(VulkanInstance* instance);


	private:
		Invision::VulkanInstance *vulkanInstance;

	};

}

#endif //