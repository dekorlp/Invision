#pragma once
#include <unordered_map>

#include "EntityService.h"

using Entity = size_t;


template <typename T>
class ComponentArray : public IComponentArray
{

private:
	std::vector<T> components;
	std::vector<Entity> entities;
	std::unordered_map<Entity, size_t> entityToIndex;
	std::unordered_map<size_t, Entity> IndexToEntity;
public:

	void InsertComponent(Entity entity, T component)
	{
		size_t index = components.size();
		components.push_back(component);
		entities.push_back(entity);
		entityToIndex[entity] = index;
		IndexToEntity[index] = entity;
	}

	T& GetComponent(Entity entity)
	{
		assert(entityToIndex.find(entity) != entityToIndex.end() && "Component does not exist!");
		return components[entityToIndex[entity]];
	}

	void RemoveComponent(Entity entity)
	{
		size_t index = entityToIndex[entity];
		size_t lastIndex = components.size() - 1;

		components[index] = components[lastIndex];
		entities[index] = entities[lastIndex];

		Entity lastEndity = IndexToEntity[lastIndex];
		entityToIndex[lastEndity] = index;
		IndexToEntity[index] = lastEndity;

		components.pop_back();
		entities.pop_back();
		entityToIndex.erase(entity);
		IndexToEntity.erase(lastIndex);
	}

	void EntityDestroyed(Entity entity) override
	{
		if (entityToIndex.find(entity) != entityToIndex.end())
		{
			RemoveComponent(entity);
		}
	}
	
};
