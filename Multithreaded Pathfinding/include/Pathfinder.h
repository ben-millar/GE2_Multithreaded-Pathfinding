#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <stack>

#include "Globals.h"
#include "Graph.h"

class Pathfinder
{
public:
	using Path = std::stack<int>;

	Pathfinder(int t_rows, int t_cols);
	~Pathfinder();

	void findPath(int t_origin, int t_destination, std::shared_ptr<Path> t_path, Graph* t_graph);

private:

	std::mutex m_mtx;

	const int ROWS, COLS;
	const int MAX_STEPS;

	const bool PREFER_ORGANIC{ false };

	// Cost of pathing through this node
	float* m_distance;

	// A function of path cost and euclidian distance
	float* m_heuristic;

	// Node we reached this one from
	int* m_previous;

	// Has this node been visited
	bool* m_visited;
};

#endif