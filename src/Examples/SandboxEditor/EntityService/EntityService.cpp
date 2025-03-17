#include "EntityService.h"

EntityService::EntityService(ServiceLocator& loc): Service(loc)
{
}

Entity EntityService::CreateEntity()
{
	return nextEntity++;
}

void EntityService::DestroyEntity(Entity entity)
{
	for (auto it = componentArrays.begin(); it != componentArrays.end(); ++it) {
		it->second->EntityDestroyed(entity);
	}
}
