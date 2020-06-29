#ifndef IGRAPHICS_ENGINE_H
#define IGRAPHICS_ENGINE_H

#include <string>
#include "IRenderer.h"


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
		 INVISION_API IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version, CanvasDimensions canvas);

		 INVISION_API EngineType::Type type() const;

		 INVISION_API std::string const& name() const;

		 INVISION_API std::string const& version() const;

		 INVISION_API virtual std::shared_ptr<IRenderer> create_renderer() = 0;

		 INVISION_API ~IGraphicsEngine();
	private:
		EngineType::Type Type_ = EngineType::Unknown;
		std::string const Name_;
		std::string const Version_;
	};

}

#endif // IGRAPHICS_ENGINE_H