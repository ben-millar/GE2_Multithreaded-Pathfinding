#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <bitset>
#include <queue>
#include <stack>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Globals.h"
#include "Node.h"

using namespace std;

class Graph
{
public:
	Graph(int t_rows, int t_cols);

	~Graph();

	void generateGraph();

private:

	/// <summary>
	/// Returns the indices of a given nodes' neighbours.
	/// If the 't_all' argument is provided, non-traversible neighbours will be included.
	/// </summary>
	/// <param name="t_row">Row of node to find neighbours of</param>
	/// <param name="t_col">Col of node to find neighbours of</param>
	/// <param name="t_vec">Vector to add neighbour indices to</param>
	/// <param name="t_all">If this is false, we only provide traversible neighbours</param>
	void neighbours(int t_row, int t_col, std::vector<int>& t_vec, bool t_all = false);

	const int ROWS, COLS;
	Node* nodes;
};

#endif