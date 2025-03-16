#ifndef ENTITY_SERVICE_H
#define ENTITY_SERVICE_H
#include "Examples/SandboxEditor/ServiceLocator/Service.h"

class EntityService : public Service
{
public:
	explicit EntityService(ServiceLocator& loc)
		: Service(loc)
	{
	}
};

#endif