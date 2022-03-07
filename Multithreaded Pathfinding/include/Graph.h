#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <bitset>
#include <vector>

#include "Globals.h"

class Graph
{
public:
	Graph(int rows, int cols);

	~Graph();

	void generateGraph();

	void neighbours(int row, int col, std::vector<int>& vec);

	const int ROWS, COLS;

	std::vector<int>* m_neighbours;
	int* m_heuristic;
	int* m_distance;
	bool* m_isTraversible;

private:
};

#endif