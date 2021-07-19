#include "print.h"

void print(std::string const& str, unsigned int const& which) {
	switch (which)
	{
	case INFO:
		std::cout << "\x1B[36m[info]\x1B[0m : ";
		break;
	case DEBUG:
		std::cout << "\x1B[33m[debug]\x1B[0m : ";
		break;
	case ERROR:
		std::cout << "\x1B[31m[error]\x1B[0m : ";
		break;
	case OK:
		std::cout << "\x1B[32m[ok]\x1B[0m : ";
		break;
	default:
		std::cout << "\x1B[36m[info]\x1B[0m : ";
		break;
	}
	std::cout << str << std::endl;
}