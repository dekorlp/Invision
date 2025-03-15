#ifndef RENDER_SERVICE_H
#define RENDER_SERVICE_H
#include "Examples/SandboxEditor/ServiceLocator/Service.h"

class RenderService : public Service
{
public:
	explicit RenderService(ServiceLocator& loc);

	void Render();

	void HalloWelt();
};

#endif