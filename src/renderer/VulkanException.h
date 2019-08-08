/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanException
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION

#include <stdexcept>
#include <map>
#include <string>
#include "vulkan/vulkan.h"

class VulkanException :
	public std::runtime_error
{
public:
	VulkanException(const VkResult status, const std::string& msg);
	VulkanException(const std::string& msg);
	virtual ~VulkanException();
	const std::string GetStatus() const;

private:
	const VkResult m_status;
	static std::map<VkResult, std::string> m_statuses;
};

#endif // INCLUDED_EXCEPTION