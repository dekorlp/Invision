/////////////////////////////////////////////////////////////////////////////
// Name:	  Log
// Purpose:	  offers logging handling operations
// Author:	  Dennis Koehler
// Created:	  2015-09-27
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef LOG_H
#define LOG_H

class HAL_API Log
{
	public:
		Log(std::string filename);
		~Log();
		void RawText(std::string message);
		void Info(std::string message);
		void Error(std::string message);
		void Warning(std::string message);
	private:
		std::ofstream stream;
		std::string filename;
};

#endif // LOG_H