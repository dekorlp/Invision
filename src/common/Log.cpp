#include "precompiled.h"
#include "Log.h"
namespace Invision {
	Log* Log::m_pThis = NULL;
	bool Log::isSet = false;

	Log::Log()
	{

	}

	Log::Log(std::string filename)
	{
		Open(filename);
	}

	void Log::Open(std::string filename)
	{
		this->filename = filename;
		struct stat info;

		const char* dirPath = "../../../Logs";

		// create dir Logs if there is no Logs directory
#ifdef _WIN32
		if (stat(dirPath, &info) != 0) _mkdir("../../../Logs");
#else
		if (stat(dirPath, &info) != 0) mkdir("../../../Logs");
#endif

		// create log and replace new content
		stream.open(filename, std::ios::out | std::ios::trunc);

		if (stream.fail())
		{
			stream.flush();
			stream.close();
			throw InvisionException(std::string("Cannot open File " + filename).c_str());
		}
	}

	void Log::RawText(std::string message)
	{
		if (Log::isSet == false)
		{
			throw InvisionMemoryNoLogObject("No Log Object assigned. Use SetLogger() to set a Log Object first!");
		}

		if (stream.is_open())
		{
			stream << message << std::endl;
		}
		else
		{
			throw InvisionException(std::string("Cannot access File " + this->filename).c_str());
		}
	}

	void Log::Info(std::string message)
	{
		if (Log::isSet == false)
		{
			throw InvisionMemoryNoLogObject("No Log Object assigned. Use SetLogger() to set a Log Object first!");
		}

		if (stream.is_open())
		{
			stream << "INFO: " << message << std::endl;
		}
		else
		{
			throw InvisionException(std::string("Cannot access File " + this->filename).c_str());
		}
	}

	void Log::Error(std::string message)
	{
		if (Log::isSet == false)
		{
			throw InvisionMemoryNoLogObject("No Log Object assigned. Use SetLogger() to set a Log Object first!");
		}

		if (stream.is_open())
		{
			stream << "Error: " << message << std::endl;
		}
		else
		{
			throw InvisionException(std::string("Cannot access File " + this->filename).c_str());
		}
	}

	void Log::Warning(std::string message)
	{
		if (Log::isSet == false)
		{
			throw InvisionMemoryNoLogObject("No Log Object assigned. Use SetLogger() to set a Log Object first!");
		}

		if (stream.is_open())
		{
			stream << "Warning: " << message << std::endl;
		}
		else
		{
			throw InvisionException(std::string("Cannot access File " + this->filename).c_str());
		}
	}

	void Log::WriteToLog(std::string initMessage, void* address)
	{
		if (Log::isSet == false)
		{
			throw InvisionMemoryNoLogObject("No Log Object assigned. Use SetLogger() to set a Log Object first!");
		}

		std::stringstream ss;
		ss << initMessage << "0x" << std::hex << address;
		INVISION_LOG_RAWTEXT(ss.str());
	}

	void Log::WriteToLog(std::string initMessage, size_t number)
	{
		if (Log::isSet == false)
		{
			throw InvisionMemoryNoLogObject("No Log Object assigned. Use SetLogger() to set a Log Object first!");
		}

		std::stringstream ss;
		ss << initMessage << number;
		INVISION_LOG_RAWTEXT(ss.str());
	}

	Log* Log::GetLogger() {
		return m_pThis;
	}

	void Log::SetLogger(Log* log) {
		Log::m_pThis = log;
		Log::isSet = true;
	}

	void Log::Close()
	{
		stream.flush();
		stream.close();
		Log::m_pThis = NULL;
		Log::isSet = false;
	}

	Log::~Log()
	{
		Close();
	}

}