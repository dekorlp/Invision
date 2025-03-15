#include "RenderService.h"

#include "ConfigService/ConfigService.h"
#include "ConfigService/StringValue.h"

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
	std::cout << "WindowTitle: " <<  serviceLocator.GetService<ConfigService>()->GetConfig<StringValue>("windowTitle")->get()<< std::endl;
}
