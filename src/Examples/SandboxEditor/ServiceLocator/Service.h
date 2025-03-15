#ifndef ISYSTEM_H
#define ISYSTEM_H

class ServiceLocator;

class Service
{
protected:
	ServiceLocator& serviceLocator;

public:
	explicit Service(ServiceLocator& loc);

	virtual ~Service();
};

#endif