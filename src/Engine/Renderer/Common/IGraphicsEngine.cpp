#include "precompiled.h"

#include "IGraphicsEngine.h"
namespace Invision
{
	IGraphicsEngine::IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version) :
		mType(type),
		mName(name),
		mVersion(version)
	{
	}

	IGraphicsEngine::IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version, std::ofstream* ofstr) :
		mType(type),
		mName(name),
		mVersion(version)
	{
	}

	EngineType::Type IGraphicsEngine::GetType() const
	{
		return mType;
	}

	std::string const& IGraphicsEngine::GetName() const
	{
		return mName;
	}

	std::string const& IGraphicsEngine::GetVersion() const
	{
		return mVersion;
	}


	IGraphicsEngine::~IGraphicsEngine()
	{

	}

}