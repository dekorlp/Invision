#pragma once
#include "Examples/SandboxEditor/ServiceLocator/Service.h"

class IRenderService : public Service
{
public:
	explicit IRenderService(ServiceLocator& loc)
		: Service(loc)
	{
	}

	virtual void Render() = 0;
};
