#ifndef BOOL_VALUE_H
#define BOOL_VALUE_H
#include <iostream>

#include "ConfigValue.h"


class BoolValue : public ConfigValue {
public:
    explicit BoolValue(bool v) : value(v) {}

    void Print() const override {
        std::cout << value << '\n';
    }

    bool get() const { return value; }

private:
    bool value;
};

#endif