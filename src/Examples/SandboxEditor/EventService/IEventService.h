#ifndef IEVENT_SERVICE_H
#define IEVENT_SERVICE_H
#include "Examples/SandboxEditor/ServiceLocator/Service.h"

class IEventService : public Service
{
public:
	explicit IEventService(ServiceLocator& loc)
		: Service(loc)
	{
	}

	virtual void InitEventLoop() = 0;
};
#endif
