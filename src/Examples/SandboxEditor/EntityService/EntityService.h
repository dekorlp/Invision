#ifndef ENTITY_SERVICE_H
#define ENTITY_SERVICE_H
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentArray.h"
#include "IComponentArray.h"
#include "Examples/SandboxEditor/ServiceLocator/Service.h"

using Entity = size_t;

template <typename T> class ComponentArray;

class EntityService : public Service
{
private:
	std::size_t nextEntity = 0;
	std::unordered_map<std::size_t, std::shared_ptr<IComponentArray>> componentArrays;

	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		std::size_t typeID = typeid(T).hash_code();
		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeID]);
	}

public:
	explicit EntityService(ServiceLocator& loc);

	Entity CreateEntity();

	template<typename T>
	void RegisterComponent()
	{
		std::size_t typeID = typeid(T).hash_code();

		std::shared_ptr<IComponentArray> componentArray = std::static_pointer_cast<IComponentArray>(
			std::make_shared<ComponentArray<T>>()
		);

		componentArrays[typeID] = componentArray;
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertComponent(entity, component);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetComponent(entity);
	}

	void DestroyEntity(Entity entity);
};

#endif