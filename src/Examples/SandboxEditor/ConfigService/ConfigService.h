#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "ConfigValue.h"
#include "Examples/SandboxEditor/Service.h"

class ConfigService : public Service
{
public:
	ConfigService(ServiceLocator& loc)
		: Service(loc)
	{
	}

	void SetConfig(const std::string& name, std::shared_ptr < ConfigValue> value)
	{
		configValueMap[name] = std::move(value);
	}

	template<typename T>
	std::shared_ptr<T> GetConfig(const std::string& name) const
	{
		auto it = configValueMap.find(name);
		if (it != configValueMap.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr;
	}

	void PrintConfig(const std::string& name) const
	{
		auto it = configValueMap.find(name);
		if (it != configValueMap.end())
		{
			it->second->Print();
		} else
		{
			std::cout << "Config parameter '" << name << "' not found.\n";
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<ConfigValue>> configValueMap;
};

#endif
