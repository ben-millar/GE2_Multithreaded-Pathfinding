#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <stack>

#include "Globals.h"
#include "Graph.h"

class Pathfinder
{
public:
	using Point = Graph::Point;
	using Path = std::stack<int>;

	Pathfinder(int t_rows, int t_cols);
	~Pathfinder();

	Path findPath(Point t_origin, Point t_destination, Graph const* t_graph);

private:

	const int ROWS, COLS;

	// Cost of pathing through this node
	float* m_distance;

	// Cost of pathing through this node
	int* m_cost;

	// A function of path cost and euclidian distance
	float* m_heuristic;

	// Node we reached this one from
	int* m_previous;

	// Has this node been visited
	bool* m_visited;
};

#endif