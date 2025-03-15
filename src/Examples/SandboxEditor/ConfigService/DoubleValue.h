#ifndef DOUBLE_VALUE_H
#define DOUBLE_VALUE_H
#include <iostream>

#include "ConfigValue.h"


class DoubleValue : public ConfigValue
{
public:
    explicit DoubleValue(double v) : value(v) {}

    void Print() const override {
        std::cout << value << '\n';
    }

    double get() const { return value; }

private:
    bool value;
};

#endif
