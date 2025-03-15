#include "RenderService.h"

RenderService::RenderService(ServiceLocator& loc): Service(loc)
{
}

void RenderService::Render()
{
	RenderService* test = serviceLocator.GetService<RenderService>();
	test->HalloWelt();
	int test2 = 0;
}

void RenderService::HalloWelt()
{
	std::cout << "Hallo Welt" << std::endl;
}
