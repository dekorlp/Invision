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

class  Log
{
	public:
		HAL_API Log(std::string filename);
		HAL_API void Open(std::string filename);
		HAL_API void RawText(std::string message);
		HAL_API void Info(std::string message);
		HAL_API void Error(std::string message);
		HAL_API void Warning(std::string message);
		HAL_API void WriteToLog(std::string initMessage, void* address);
		HAL_API void WriteToLog(std::string initMessage, size_t number);
		HAL_API void Close();
		HAL_API ~Log();

		HAL_API static Log* GetLogger();
		HAL_API static void SetLogger(Log* log);
		HAL_API static Log* m_pThis;
		HAL_API static bool isSet;
		
	private:
		std::ofstream stream;
		std::string filename;

		
};

#define INVISION_LOG_RAWTEXT(...) Log::GetLogger()->RawText(__VA_ARGS__);
#define INVISION_LOG_ERROR(...) Log::GetLogger()->Error(__VA_ARGS__);
#define INVISION_LOG_WARNING(...) Log::GetLogger()->Warning(__VA_ARGS__);
#define INVISION_LOG_INFO(...) Log::GetLogger()->Info(__VA_ARGS__);

#endif // LOG_H