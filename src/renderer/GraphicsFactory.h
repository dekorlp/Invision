#ifndef GRAPHICS_FACTORY_H
#define GRAPHICS_FACTORY_H
#include "VulkanEngine.h"
//#include "DirectXEngine.h"

namespace Invision
{
	INVISION_API std::shared_ptr<IGraphicsEngine> create_engine(EngineType::Type type);
}
#endif // GRAPHICS_FACTORY_H
