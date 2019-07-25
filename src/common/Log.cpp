#include "precompiled.h"

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

Log::~Log()
{
	stream.flush();
	stream.close();
}