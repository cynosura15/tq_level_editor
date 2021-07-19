#pragma once
#include <iostream>

#define INFO 0
#define DEBUG 1
#define ERROR 2
#define OK 3

extern void print(std::string const& str, unsigned int const& which = 9);