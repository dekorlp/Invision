#ifndef IWINDOW_SERVICE_H
#define IWINDOW_SERVICE_H
#include "Examples/SandboxEditor/Service.h"

class IWindowService : public Service
{
public:
	explicit IWindowService(ServiceLocator& loc)
		: Service(loc)
	{
	}

	virtual int InitWindow(const char* title, int width, int height) = 0;
	virtual void* getHandle() = 0;
	virtual int Destroy() = 0;
};
#endif