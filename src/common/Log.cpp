#include "precompiled.h"

Log* Log::m_pThis = NULL;

Log::Log(std::string filename)
{
	this->filename = filename;
	struct stat info;

	const char* dirPath = "../../../Logs";

	// create dir Logs if there is no Logs directory
	if(stat(dirPath, &info) != 0) mkdir("../../../Logs");

	// create log and replace new content
	stream.open(filename, std::ios::out | std::ios::trunc);

	if (stream.fail())
	{
		stream.flush();
		stream.close();
		throw invisionCoreException(std::string("Cannot open File " + filename).c_str());
	}
}

void Log::RawText(std::string message)
{
	if (stream.is_open())
	{
		stream << message << std::endl;
	}
	else
	{
		throw invisionCoreException(std::string("Cannot access File " + this->filename).c_str());
	}
}

void Log::Info(std::string message)
{
	if (stream.is_open())
	{
		stream << "INFO: " << message << std::endl;
	}
	else
	{
		throw invisionCoreException(std::string("Cannot access File " + this->filename).c_str());
	}
}

void Log::Error(std::string message)
{
	if (stream.is_open())
	{
		stream << "Error: " << message << std::endl;
	}
	else
	{
		throw invisionCoreException(std::string("Cannot access File " + this->filename).c_str());
	}
}

void Log::Warning(std::string message)
{
	if (stream.is_open())
	{
		stream << "Warning: " << message << std::endl;
	}
	else
	{
		throw invisionCoreException(std::string("Cannot access File " + this->filename).c_str());
	}
}

void Log::WriteToLog(std::string initMessage, void* address)
{
	std::stringstream ss;
	ss << initMessage << "0x" << std::hex << address;
	INVISION_LOG_RAWTEXT(ss.str());
}

void Log::WriteToLog(std::string initMessage, size_t number)
{
	std::stringstream ss;
	ss << initMessage << number;
	INVISION_LOG_RAWTEXT(ss.str());
}

Log* Log::GetLogger() {
	return m_pThis;
}

void Log::SetLogger(Log* log) {
	Log::m_pThis = log;
}

Log::~Log()
{
	stream.flush();
	stream.close();
}