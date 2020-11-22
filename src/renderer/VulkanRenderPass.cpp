#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanRenderPass.h"
namespace Invision
{

	/*VulkanRenderPass::VulkanRenderPass(VulkanEngine* engine) :
		IRenderPass(engine)
	{
		renderPass.AddAttachment(engine->GetVulkanInstance(), engine->GetVulkanContext());
		renderPass.AddSubpass();
		renderPass.CreateRenderPass(engine->GetVulkanInstance());
		vulkanEngine = engine;
	}*/
	
	VulkanRenderPass::VulkanRenderPass(VulkanInstance* instance)
		:IRenderPass(instance)
	{
		vulkanInstance = instance;


		renderPass.AddAttachment(instance->GetCoreEngine()->GetVulkanInstance(), instance->GetVulkanContext(), instance->GetVulkanContext().swapChainImageFormat, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		if (instance->GetDepthRessources().AreDepthRessourcesActivated() == true)
		{
			renderPass.AddAttachment(instance->GetCoreEngine()->GetVulkanInstance(), instance->GetVulkanContext(), instance->GetDepthRessources().findDepthFormat(instance->GetCoreEngine()->GetVulkanInstance()), VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}

		renderPass.AddSubpass({}, instance->GetDepthRessources().AreDepthRessourcesActivated());

		//renderPass.AddSubpassDependency(instance->GetCoreEngine()->GetVulkanInstance(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
		//	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		renderPass.AddSubpassDependency(instance->GetCoreEngine()->GetVulkanInstance(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);


		renderPass.CreateRenderPass(instance->GetCoreEngine()->GetVulkanInstance());
		
		//vulkanEngine = instance->GetCoreEngine();
	}

	VulkanBaseRenderPass VulkanRenderPass::GetRenderPass()
	{
		return renderPass;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		renderPass.DestroyRenderPass(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
}