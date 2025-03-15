#ifndef INT_VALUE_H
#define INT_VALUE_H
#include <iostream>

#include "ConfigValue.h"

class IntValue : public ConfigValue
{
private:
	int value;
public:
	explicit IntValue(int v) : value(v) {}
	void Print() const override
	{
		std::cout << value << "\n";
	}

	int get() const { return value; }
};

#endif