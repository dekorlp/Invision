#include "RenderService.h"

#include <iostream>

#include "Examples/SandboxEditor/ConfigService/ConfigService.h"
#include "Examples/SandboxEditor/ConfigService/StringValue.h"
#include "Examples/SandboxEditor/ServiceLocator/ServiceLocator.h"

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
