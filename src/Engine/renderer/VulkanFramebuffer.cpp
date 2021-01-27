#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"

namespace Invision
{

	VulkanFramebuffer::VulkanFramebuffer(VulkanInstance* instance, std::shared_ptr<Invision::IRenderPass> renderPass) :
		IFramebuffer(instance, renderPass)
	{

		vulkanInstance = instance;
		
	}

	void VulkanFramebuffer::CreateMainFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass)
	{
		mFramebuffers.resize(vulkanInstance->GetVulkanContext().swapChainImageViews.size());

		if (vulkanInstance->GetDepthRessources().AreDepthRessourcesActivated())
		{
			for (int i = 0; i < vulkanInstance->GetVulkanContext().swapChainImageViews.size(); i++)
			{
				std::vector< VkImageView> attachments;
				attachments.push_back(vulkanInstance->GetVulkanContext().swapChainImageViews[i]);
				attachments.push_back(vulkanInstance->GetDepthRessources().GetDepthImageView());

				mFramebuffers[i].CreateFramebuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext(),  dynamic_pointer_cast<Invision::VulkanRenderPass>(renderPass)->GetRenderPass(), attachments);
			}
		}
		else
		{
			for (int i = 0; i < vulkanInstance->GetVulkanContext().swapChainImageViews.size(); i++)
			{
				std::vector< VkImageView> attachments;
				attachments.push_back(vulkanInstance->GetVulkanContext().swapChainImageViews[i]);

				mFramebuffers[i].CreateFramebuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext(), dynamic_pointer_cast<Invision::VulkanRenderPass>(renderPass)->GetRenderPass(), attachments);
			}
		}
	}

	VulkanBaseFramebuffer VulkanFramebuffer::GetFramebuffer(unsigned int index)
	{
		return mFramebuffers[index];
	}

	std::vector < VulkanBaseFramebuffer> VulkanFramebuffer::GetFramebuffers()
	{
		return mFramebuffers;
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		for (int i = 0; i < mFramebuffers.size(); i++)
		{
			mFramebuffers[i].DestroyFramebuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
		}
	}

}