#ifndef STRING_VALUE_H
#define STRING_VALUE_H
#include <iostream>

#include "ConfigValue.h"

class StringValue : public ConfigValue
{
private:
	std::string value;
public:
	explicit StringValue(std::string v) : value(std::move(v)) {}
	void Print() const override
	{
		std::cout << value << "\n";
	}

	const std::string& get() const { return value; }
};

#endif