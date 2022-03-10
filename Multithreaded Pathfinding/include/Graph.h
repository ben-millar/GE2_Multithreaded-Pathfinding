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

	Graph(int t_rows, int t_cols);

	~Graph();

	void generateGraph();

	size_t mouseClickToIndex(sf::Vector2f t_position);

	std::vector<int> const* getNeighbours() const { return m_neighbours; };

	float distanceBetweenPoints(int t_origin, int t_destination) const;

	size_t pointToIndex(Point t_point) const { return t_point.y * COLS + t_point.x; };

private:

	void neighbours(int t_row, int t_col, std::vector<int>& t_vec);

	const int ROWS, COLS;

	// Pre-bake the valid neighbours of each node
	std::vector<int>* m_neighbours;

	// Is this node traversible
	bool* m_isTraversible;
};

#endif