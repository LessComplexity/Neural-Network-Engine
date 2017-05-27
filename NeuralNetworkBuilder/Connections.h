#pragma once
#include "definitions.h"

class Connections
{
public:
	Connections();
	~Connections();
	Connection &operator[](int i);
private:
	std::vector<Connection> connections;
};

