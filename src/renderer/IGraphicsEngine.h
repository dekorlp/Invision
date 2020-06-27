#ifndef IGRAPHICS_ENGINE_H
#define IGRAPHICS_ENGINE_H

#include <string>

namespace Invision
{


	struct CanvasDimensions
	{
		HWND hwnd;
		int width;
		int height;

	};

	namespace EngineType
	{
		enum Type
		{
			Unknown,
			DirectX,
			Vulkan
		};
	}

	class IGraphicsEngine
	{
	public:
		IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version, CanvasDimensions canvas);

		EngineType::Type type() const;

		std::string const& name() const;

		std::string const& version() const;

		~IGraphicsEngine();
	private:
		EngineType::Type Type_ = EngineType::Unknown;
		std::string const Name_;
		std::string const Version_;
	};

}

#endif // IGRAPHICS_ENGINE_H