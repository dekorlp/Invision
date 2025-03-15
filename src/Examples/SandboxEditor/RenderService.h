#ifndef RENDER_SERVICE_H
#define RENDER_SERVICE_H
#include <iostream>

#include "Service.h"
#include "ServiceLocator.h"

class RenderService : public Service
{
public:
	explicit RenderService(ServiceLocator& loc);

	void Render();

	void HalloWelt();
};

#endif