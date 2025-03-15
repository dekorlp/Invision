#include "ServiceLocator.h"

void ServiceLocator::RegisterSystem(Service* service)
{
	serviceMap[typeid(*service)] = service;
};