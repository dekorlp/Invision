#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanRender.h"
namespace Invision
{

	/*VulkanRenderer::VulkanRenderer(VulkanEngine* engine) :
		IRenderer(engine)
	{
	}*/

	VulkanRenderer::VulkanRenderer(VulkanEngine* engine)
		: IRenderer(engine)
	{
		
	}

	void VulkanRenderer::render(std::shared_ptr<ICommandBuffer> commandBuffer)
	{
		
	}
}