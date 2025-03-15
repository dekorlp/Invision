#ifndef CONFIG_VALUE_H
#define CONFIG_VALUE_H

class ConfigValue
{
public:
	virtual ~ConfigValue() = default;
	virtual void Print() const = 0;
};

#endif
