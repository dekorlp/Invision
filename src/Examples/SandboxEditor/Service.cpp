#include "ServiceLocator.h"
#include "Service.h"

Service::Service(ServiceLocator& loc): serviceLocator(loc)
{

}

Service::~Service()
{}
