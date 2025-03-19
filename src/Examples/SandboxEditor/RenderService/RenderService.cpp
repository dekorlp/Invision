#include "../../../../build/solutions/vs2022/VulkanRenderService/RenderService.h"


#include <iostream>

#include "Examples/SandboxEditor/ConfigService/ConfigService.h"
#include "Examples/SandboxEditor/ConfigService/StringValue.h"
#include "Examples/SandboxEditor/ServiceLocator/ServiceLocator.h"

//VulkanRenderService::VulkanRenderService(ServiceLocator& loc): Service(loc)
//{
//}

void VulkanRenderService::Render()
{
	VulkanRenderService* test = serviceLocator.GetService<VulkanRenderService>();
	test->HalloWelt();
	int test2 = 0;
}

void VulkanRenderService::HalloWelt()
{
	std::cout << "WindowTitle: " <<  serviceLocator.GetService<ConfigService>()->GetConfig<StringValue>("windowTitle")->get()<< std::endl;
}
