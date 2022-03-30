#ifndef ARC_H
#define ARC_H

#include <cstdint>
#include <vector>

class Node;

class Arc
{
	uint32_t cost{ 0 };
	Node* destination;
};

#endif