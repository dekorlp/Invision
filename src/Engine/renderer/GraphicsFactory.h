#ifndef GRAPHICS_FACTORY_H
#define GRAPHICS_FACTORY_H
#include "VulkanEngine.h"
//#include "DirectXEngine.h"

namespace Invision
{
	INVISION_API std::shared_ptr<IGraphicsEngine> Create_engine(EngineType::Type type);
	INVISION_API std::shared_ptr<IGraphicsEngine> Create_engine(EngineType::Type type, std::ofstream* ofstr);
}
#endif // GRAPHICS_FACTORY_H
