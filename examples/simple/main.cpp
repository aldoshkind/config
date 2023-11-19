
#include <iostream>

class simple_logger
{
public:
	~simple_logger()
	{
		std::cout << std::endl;
	}

	template <class T>
	simple_logger &operator << (const T &op)
	{
		std::cout << op;
		return *this;
	}
};

#undef ADN_CONFIG_USE_DUMMY_LOGGER
#define LOG_I simple_logger()
#include "config.hpp"

#include <stdio.h>

int main()
{
	auto &c = adn::config::examples::config::instance();

	c.dump();

	printf("String value: %s\n", c.string_value().c_str());
	printf("Integer value: %d\n", c.integer_value());
	printf("Bool value: %s\n", c.bool_value() ? "true" : "false");
	printf("Json value: %s\n", c.json_value().dump(1).c_str());

	return 0;
}