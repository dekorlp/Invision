#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H
#include <map>
#include <typeindex>

#include "Service.h"

class ServiceLocator
{
private:
	std::map<std::type_index, Service*> serviceMap;

public:

	void RegisterSystem(Service* service);

	template <typename T>
	T* GetService()
	{
		auto it = serviceMap.find(typeid(T));
		if (it != serviceMap.end())
		{
			return dynamic_cast<T*>(it->second);
		}
		return nullptr;
	}
};
#endif