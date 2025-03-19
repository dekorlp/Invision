#pragma once
#include "IRenderService.h"

class VulkanRenderService : public IRenderService
{
public:
	explicit VulkanRenderService(ServiceLocator& loc)
		: IRenderService(loc)
	{
	}

	void Render() override;
};
