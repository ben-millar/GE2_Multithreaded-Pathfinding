#include "Graph.h"

Graph::Graph(int t_rows, int t_cols) :
	ROWS(t_rows),
	COLS(t_cols)
{
	m_neighbours = new std::vector<int>[ROWS * COLS];

	m_cost = new int[ROWS * COLS];
	std::fill_n(m_cost, ROWS * COLS, 0);

	m_isTraversible = new bool[ROWS * COLS];
	std::fill_n(m_isTraversible, ROWS * COLS, true);

	generateGraph();
}

////////////////////////////////////////////////////////////

Graph::~Graph()
{
	delete[] m_neighbours;
	delete[] m_cost;
	delete[] m_isTraversible;
}

////////////////////////////////////////////////////////////

void Graph::generateGraph()
{
	for (int row = 0; row < ROWS; ++row)
		for (int col = 0; col < COLS; ++col)
			neighbours(row, col, m_neighbours[pointToIndex({ row, col })]);
}

////////////////////////////////////////////////////////////

size_t Graph::mouseClickToIndex(sf::Vector2f t_position)
{
	int row = t_position.y / ROWS;
	int col = t_position.x / COLS;

	return pointToIndex({ row, col });
}

////////////////////////////////////////////////////////////

void Graph::toggleWall(int t_index)
{
	// Set our point to be either traversible or not
	m_isTraversible[t_index] = !m_isTraversible[t_index];
	Point p = indexToPoint(t_index);

	std::vector<int> all_neighbours;
	neighbours(p.row, p.col, all_neighbours, true);

	for (auto& n : all_neighbours)
	{
		std::vector<int>& v = m_neighbours[n];

		if (m_isTraversible[t_index]) // If we're removing a wall, add it to neighbour lists
			v.push_back(t_index);
		else // Otherwise, if we're setting this as a wall, remove from neighbour lists
			v.erase(std::remove(v.begin(), v.end(), t_index), v.end());
	}
}

////////////////////////////////////////////////////////////

float Graph::distanceBetweenIndices(int t_origin, int t_destination) const
{
	Point deltaPos = indexToPoint(t_destination) - indexToPoint(t_origin);

	return sqrt(deltaPos.row * deltaPos.row + deltaPos.col * deltaPos.col);
}

////////////////////////////////////////////////////////////

void Graph::neighbours(int t_row, int t_col, std::vector<int>& t_vec, bool t_all)
{
	int _row, _col;

	for (int i = -1; i <= 1; ++i)
	{
		_row = t_row + i;
		if (_row < 0 || _row >= ROWS) continue;

		for (int j = -1; j <= 1; ++j)
		{
			_col = t_col + j;

			if (_col < 0 || _col >= COLS) continue;
			if (_row == t_row && _col == t_col) continue;

			if (!t_all)
				if (!m_isTraversible[pointToIndex({ _row, _col })]) continue;

			t_vec.push_back(pointToIndex({ _row, _col }));
		}
	}
}
