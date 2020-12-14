#include "precompiled.h"

#include "IGraphicsEngine.h"
namespace Invision
{
	IGraphicsEngine::IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version) :
		Type_(type),
		Name_(name),
		Version_(version)
	{
	}

	IGraphicsEngine::IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version, std::ofstream* ofstr) :
		Type_(type),
		Name_(name),
		Version_(version)
	{
	}

	EngineType::Type IGraphicsEngine::type() const
	{
		return Type_;
	}

	std::string const& IGraphicsEngine::name() const
	{
		return Name_;
	}

	std::string const& IGraphicsEngine::version() const
	{
		return Version_;
	}


	IGraphicsEngine::~IGraphicsEngine()
	{

	}

}