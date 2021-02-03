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

		mVulkanInstance = instance;
		
	}

	void VulkanFramebuffer::CreateMainFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass)
	{
		mFramebuffers.resize(mVulkanInstance->GetVulkanContext().swapChainImageViews.size());

		
		for (int i = 0; i < mVulkanInstance->GetVulkanContext().swapChainImageViews.size(); i++)
		{

			std::vector< VkImageView> attachments;

			
			if (mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().UseMSAA == false)
			{
				attachments.push_back(mVulkanInstance->GetVulkanContext().swapChainImageViews[i]);
				if (mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated())
				{
					attachments.push_back(mVulkanInstance->GetDepthRessources().GetDepthImageView());
				}
			}
			else
			{
				attachments.push_back(mVulkanInstance->GetColorRessources().GetColorImageView());

				if (mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated())
				{
					attachments.push_back(mVulkanInstance->GetDepthRessources().GetDepthImageView());
				}
				attachments.push_back(mVulkanInstance->GetVulkanContext().swapChainImageViews[i]);
			}



			mFramebuffers[i].CreateFramebuffer(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(),  dynamic_pointer_cast<Invision::VulkanRenderPass>(renderPass)->GetRenderPass(), attachments);
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
			mFramebuffers[i].DestroyFramebuffer(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
		}
	}

}