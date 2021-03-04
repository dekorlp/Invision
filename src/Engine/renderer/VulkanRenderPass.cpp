#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"
#include "VulkanTexture.h"

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
		mVulkanInstance = instance;

		
		
		//vulkanEngine = instance->GetCoreEngine();
	}

	void VulkanRenderPass::CreateMainRenderPass(VulkanBaseTexture& depthTexture, VulkanBaseTexture& colorTexture)
	{
		mIsMainRenderpass = true;

		// Create RenderPass with MSAA

		VulkanBaseSubPass basePass;
		basePass.mIsMainSubPass = true;
		
		basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
			mVulkanInstance->GetVulkanContext(),
			mVulkanInstance->GetVulkanContext().swapChainImageFormat,
			mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
		mAttachmentRefTextures.push_back(&colorTexture);

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
		{
			basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetDepthRessources().FindDepthFormat(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct()),
				mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				{ mAttachmentIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(&depthTexture);
		}

		basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
			mVulkanInstance->GetVulkanContext(),
			mVulkanInstance->GetVulkanContext().swapChainImageFormat,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, true);
		

		mRenderPass.AddSubpass(basePass);

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
		{
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
		}
		else
		{
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		}




		mRenderPass.CreateRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}

	void VulkanRenderPass::CreateMainRenderPass(VulkanBaseTexture& depthTexture)
	{
		mIsMainRenderpass = true;

		// Create RenderPass without MSAA

		std::vector<Invision::VulkanBaseTexture> baseTextures;

		VulkanBaseSubPass basePass;
		basePass.mIsMainSubPass = true;

		basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
			mVulkanInstance->GetVulkanContext(),
			mVulkanInstance->GetVulkanContext().swapChainImageFormat,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
		{
			basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetDepthRessources().FindDepthFormat(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct()),
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				{ mAttachmentIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(&depthTexture);
		}

		mRenderPass.AddSubpass(basePass);

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
		{
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
		}
		else
		{
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		}




		mRenderPass.CreateRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}

	void VulkanRenderPass::AddAttachment(AttachmentType attachmentType, std::shared_ptr<ITexture>& attachment)
	{
		// !!! Actually there is only one Subpass supported !!!
		// More than one subpasses are usefull for smartphone devices!
		if (mSubPass.size() == 0)
		{
			VulkanBaseSubPass basePass;
			mSubPass.push_back(basePass);

			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_MEMORY_READ_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_DEPENDENCY_BY_REGION_BIT);
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), 0, VK_SUBPASS_EXTERNAL, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_DEPENDENCY_BY_REGION_BIT);
		}

		VulkanBaseTexture *baseTexture = new VulkanBaseTexture(dynamic_pointer_cast<VulkanTexture>(attachment)->GetBaseTexture());
		
		if (attachmentType == ATTACHMENT_TYPE_COLOR)
		{
			VkFormat form = baseTexture->GetFormat();

			mSubPass[0].AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetVulkanContext().swapChainImageFormat,
				mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
				, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(baseTexture);
		}
		else // ATTACHMENT_TYPE_DEPTH
		{
			mSubPass[0].AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetDepthRessources().FindDepthFormat(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct()),
				mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				{ mAttachmentIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(baseTexture);
		}

		int testet = 0;
		
	}

	void VulkanRenderPass::CreateRenderPass()
	{
		mRenderPass.AddSubpass(mSubPass[0]);
		mRenderPass.CreateRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}

	VulkanBaseRenderPass VulkanRenderPass::GetRenderPass()
	{
		return mRenderPass;
	}

	std::vector<Invision::VulkanBaseTexture*> VulkanRenderPass::GetAttachmentTextures()
	{
		return mAttachmentRefTextures;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		mRenderPass.DestroyRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
		if (!mIsMainRenderpass) // these are attachmentes -> they are allocated with new
		{
			for (int i= 0; i < mAttachmentRefTextures.size(); i++)
			{
				delete mAttachmentRefTextures[i];
			}
			mAttachmentRefTextures.clear();
		}
	}
}