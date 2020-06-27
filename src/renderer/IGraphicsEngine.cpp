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

	IGraphicsEngine::~IGraphicsEngine()
	{

	}

}