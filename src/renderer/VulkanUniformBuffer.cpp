#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanUniformBuffer.h"
namespace Invision
{

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanEngine* engine) :
		IUniformBuffer(engine)
	{
		vulkanEngine = engine;
	}

	VulkanUniformBuffer& VulkanUniformBuffer::CreateUniformBinding(uint32_t binding, uint32_t descriptorCount, ShaderStage shaderStage, uint64_t bufferSize, uint64_t offset)
	{
		VkShaderStageFlags vkShaderStage;
		

		switch (shaderStage)
		{
		case SHADER_STAGE_VERTEX_BIT:
			vkShaderStage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case SHADER_STAGE_GEOMETRY_BIT:
			vkShaderStage = VK_SHADER_STAGE_GEOMETRY_BIT;
			break;
		case SHADER_STAGE_FRAGMENT_BIT:
			vkShaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		case SHADER_STAGE_COMPUTE_BIT:
			vkShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
			break;
		default:
			throw InvisionBaseRendererException("Unknown ShaderStageFlag passed to Function CreateUniformBinding");
			
		}

		uniformBuffer.CreateUniformBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount, vkShaderStage, bufferSize, offset);
		return *this;
	}

	void VulkanUniformBuffer::CreateUniformBuffer()
	{
		uniformBuffer.CreateUniformBuffer(vulkanEngine->GetVulkanInstance());
	}

	void VulkanUniformBuffer::UpdateUniform(const void* source, size_t size, uint32_t binding)
	{
		uniformBuffer.UpdateUniform(vulkanEngine->GetVulkanInstance(), source, size, binding);
	}

	VulkanBaseUniformBuffer VulkanUniformBuffer::GetBuffer()
	{
		return uniformBuffer;
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		uniformBuffer.DestroyUniformBuffer(vulkanEngine->GetVulkanInstance());
	}
}