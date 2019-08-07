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
		INVISION_API Log(std::string filename);
		INVISION_API void Open(std::string filename);
		INVISION_API void RawText(std::string message);
		INVISION_API void Info(std::string message);
		INVISION_API void Error(std::string message);
		INVISION_API void Warning(std::string message);
		INVISION_API void WriteToLog(std::string initMessage, void* address);
		INVISION_API void WriteToLog(std::string initMessage, size_t number);
		INVISION_API void Close();
		INVISION_API ~Log();

		INVISION_API static Log* GetLogger();
		INVISION_API static void SetLogger(Log* log);
		INVISION_API static Log* m_pThis;
		INVISION_API static bool isSet;
		
	private:
		std::ofstream stream;
		std::string filename;

		
};

#define INVISION_LOG_RAWTEXT(...) Log::GetLogger()->RawText(__VA_ARGS__);
#define INVISION_LOG_ERROR(...) Log::GetLogger()->Error(__VA_ARGS__);
#define INVISION_LOG_WARNING(...) Log::GetLogger()->Warning(__VA_ARGS__);
#define INVISION_LOG_INFO(...) Log::GetLogger()->Info(__VA_ARGS__);

#endif // LOG_H