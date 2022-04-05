#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <bitset>
#include <queue>
#include <stack>
#include <mutex>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Globals.h"

struct Point {
	Point(int t_row, int t_col) :
		row(t_row), col(t_col) {}
	int row, col;

	Point operator+(Point const& rhs) {
		return Point(this->row + rhs.row, this->col + rhs.col);
	}

	void operator+=(Point const& p) {
		this->row += p.row;
		this->col += p.col;
	}

	Point operator-(Point const& rhs) {
		return Point(this->row - rhs.row, this->col - rhs.col);
	}
};

class Graph
{
public:
	Graph(int t_rows, int t_cols);

	~Graph();

	void generateGraph();

	size_t mouseClickToIndex(sf::Vector2f t_position);

	std::vector<int> const* getNeighbours() const { return m_neighbours; };

	bool const* getWalls() const { return m_isTraversible; }

	/// <summary>
	/// Sets or unsets a wall at the given point (DEFAULT: set)
	/// </summary>
	void toggleWall(int t_index);

	/// <summary>
	/// Get a pointer to the c-style cost array (mutable)
	/// </summary>
	/// <returns>Pointer to the first index in the cost array</returns>
	int const* getCost() { return m_cost; }

	/// <summary>
	/// Increase the cost of a given index
	/// </summary>
	void increaseCost(int t_index);

	/// <summary>
	/// Reset our path costs between pathing batches
	/// </summary>
	void resetCosts() { std::fill_n(m_cost, ROWS * COLS, 1); }

	/// <summary>
	/// Returns whether a given index is traversible.
	/// NO bounds checking.
	/// </summary>
	/// <param name="t_index">Index of cell to check</param>
	/// <returns>True is cell is traversible, false otherwise</returns>
	bool isTraversible(int t_index) const { return m_isTraversible[t_index]; }

	/// <summary>
	/// 
	/// </summary>
	float distanceBetweenIndices(int t_origin, int t_destination) const;

	/// <summary>
	/// Converts a point (row, col) to an index into our graph
	/// </summary>
	size_t pointToIndex(Point t_point) const { return t_point.row * COLS + t_point.col; }

	/// <summary>
	/// Converts an index into our graph to a point (row, col)
	/// </summary>
	Point indexToPoint(size_t t_index) const { return Point(t_index / COLS, t_index % COLS); }

	const int ROWS, COLS;

private:

	/// <summary>
	/// Determines the neighbours of a given row/coll and adds them to passed vec
	/// </summary>
	/// <param name="t_row">Row of cell to check</param>
	/// <param name="t_col">Col of cell t check</param>
	/// <param name="t_vec">Vector to add neighbours too</param>
	/// <param name="t_all">If true, return all neighbours even if non-traversible.
	/// If false, only return traversible neighbours.</param>
	void neighbours(int t_row, int t_col, std::vector<int>& t_vec, bool t_all = false);

	// Pre-bake the valid neighbours of each node
	std::vector<int>* m_neighbours;

	// Cost of pathing through this node
	int* m_cost;

	mutable std::mutex* m_costMutex;

	// Is this node traversible
	bool* m_isTraversible;
};

#endif