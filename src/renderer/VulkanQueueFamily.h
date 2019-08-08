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

#ifndef INCLUDED_VULKANQUEUEFAMILY
#define INCLUDED_VULKANQUEUEFAMILY

#define NOMINMAX

#include <vector>
#include <set>
#include "VulkanException.h"
#include "VulkanDataVariables.h"

class CVulkanQueueFamily
{
private:

public:
	VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR surface) const;
	std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
		const std::set<int>& uniqueQueueFamilies) const noexcept;

};

#endif // INCLUDED_VULKANQUEUEFAMILY