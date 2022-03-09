#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <bitset>
#include <queue>
#include <stack>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Globals.h"

class Graph
{
public:
	using Point = sf::Vector2i;
	using Path = std::stack<int>;

	Graph(int rows, int cols);

	~Graph();

	void generateGraph();

	size_t mouseClickToIndex(sf::Vector2f t_position);

	Path findPath(Point t_origin, Point t_destination);

private:

	size_t pointToIndex(Point t_point) { return t_point.y * COLS + t_point.x; }

	float distanceBetweenPoints(int t_origin, int t_destination);

	void neighbours(int t_row, int t_col, std::vector<int>& t_vec);

	const int ROWS, COLS;

	// Pre-bake the valid neighbours of each node
	std::vector<int>* m_neighbours;

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

	// Is this node traversible
	bool* m_isTraversible;
};

#endif