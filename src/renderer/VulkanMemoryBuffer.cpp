/* Copyright (C) 2019 Wildfire Games.
* This file is part of 0 A.D.
*
* 0 A.D. is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* 0 A.D. is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "precompiled.h"

#include "VulkanMemoryBuffer.h"

uint32_t CVulkanMemoryBuffer::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw CVulkanException("Failed to find suitable memory type!");
}

	void CVulkanMemoryBuffer::createVertexBuffer(VkPhysicalDevice physicalDevice, VulkanLogicalDevice logicalDevice, std::vector<ShaderPipeline> *shaderPipeline)
	{
		for (unsigned int i = 0; i < shaderPipeline->size(); i++)
		{
			if (shaderPipeline->at(i).vertexSource == IN_VERTEX_SOURCE_STRUCT)
			{
				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = sizeof(shaderPipeline->at(i).meshData.meshVertizes[0]) * shaderPipeline->at(i).meshData.meshVertizes.size();
				bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				VkResult result = vkCreateBuffer(logicalDevice.m_logicalDevice, &bufferInfo, nullptr, &(shaderPipeline->at(i).meshData.VertexBuffer));
				if (result != VK_SUCCESS)
				{
					throw CVulkanException(result, "Failed to create a Vertex Buffer:");
				}

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(logicalDevice.m_logicalDevice, shaderPipeline->at(i).meshData.VertexBuffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

				VkResult result2 = vkAllocateMemory(logicalDevice.m_logicalDevice, &allocInfo, nullptr, &(shaderPipeline->at(i).meshData.vertexBufferMemory));
				if (result2 != VK_SUCCESS)
				{
					throw CVulkanException(result, "failed to allocate vertex buffer memory!");
				}

				vkBindBufferMemory(logicalDevice.m_logicalDevice, shaderPipeline->at(i).meshData.VertexBuffer, shaderPipeline->at(i).meshData.vertexBufferMemory, 0);

				void *data;
				vkMapMemory(logicalDevice.m_logicalDevice, shaderPipeline->at(i).meshData.vertexBufferMemory, 0, bufferInfo.size, 0, &data);
				memcpy(data, shaderPipeline->at(i).meshData.meshVertizes.data(), (size_t)bufferInfo.size);
				vkUnmapMemory(logicalDevice.m_logicalDevice, shaderPipeline->at(i).meshData.vertexBufferMemory);

			}
		}
	}

	void CVulkanMemoryBuffer::cleanUpBuffers(VulkanLogicalDevice logicalDevice, std::vector<ShaderPipeline> *shaderPipeline)
	{
		for (unsigned int i = 0; i < shaderPipeline->size(); i++)
		{
			if (shaderPipeline->at(i).vertexSource == IN_VERTEX_SOURCE_STRUCT)
			{
				vkDestroyBuffer(logicalDevice.m_logicalDevice, shaderPipeline->at(i).meshData.VertexBuffer, nullptr);
				vkFreeMemory(logicalDevice.m_logicalDevice, shaderPipeline->at(i).meshData.vertexBufferMemory, nullptr);
			}
		}
	}