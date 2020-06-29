#ifndef VULKAN_BASE_EXCEPTION_H
#define VULKAN_BASE_EXCEPTION_H

#include "vulkan\vulkan.h"

namespace Invision
{

	class VulkanBaseException : public Invision::InvisionBaseException
	{
	public:
		INVISION_API VulkanBaseException(const VkResult status, const std::string& msg);
		INVISION_API VulkanBaseException(const std::string& msg);
		INVISION_API virtual ~VulkanBaseException();
		INVISION_API const std::string GetStatus() const;
	private:
		const VkResult m_status;
		static std::map<VkResult, std::string> m_statuses;
	};

}

#endif // VULKAN_BASE_EXCEPTION
