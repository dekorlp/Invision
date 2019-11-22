#ifndef VULKANEXCEPTION_H
#define VULKANEXCEPTION_H

#include "vulkan\vulkan.h"
#include "common\exception.h"

namespace Invision
{

	class VulkanException : public Invision::InvisionBaseException
	{
	public:
		INVISION_API VulkanException(const VkResult status, const std::string& msg);
		INVISION_API VulkanException(const std::string& msg);
		INVISION_API virtual ~VulkanException();
		INVISION_API const std::string GetStatus() const;
	private:
		const VkResult m_status;
		static std::map<VkResult, std::string> m_statuses;
	};

}

#endif
