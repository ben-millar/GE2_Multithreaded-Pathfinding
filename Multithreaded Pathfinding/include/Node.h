#ifndef NODE_H
#define NODE_H

#include "Arc.h"

class Node
{
public:
	bool isTraversible{ true };
	std::vector<Arc> arcs;
};

#endif