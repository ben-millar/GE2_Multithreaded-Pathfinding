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
	for (int row = 0; row < COLS; ++row)
		for (int col = 0; col < ROWS; ++col)
			neighbours(row, col, m_neighbours[pointToIndex({ col, row })]);
}

////////////////////////////////////////////////////////////

size_t Graph::mouseClickToIndex(sf::Vector2f t_position)
{
	int row = t_position.y / ROWS;
	int col = t_position.x / COLS;

	return pointToIndex({ col, row });
}

////////////////////////////////////////////////////////////

void Graph::setWall(Point t_point, bool t_set)
{
	int index = pointToIndex(t_point);

	// Set our point to be either traversible or not
	m_isTraversible[index] = t_set;

	std::vector<int> v;
	neighbours(t_point.row, t_point.col, v, true);

	int _row, _col;

	if (t_set) // If we're setting this as a wall, remove from neighbour lists
		v.erase(
			std::remove_if(v.begin(), v.end(),
				[&](int const& val) {
					return val == index;
				}),
			v.end()
		);
	else // Otherwise, if we're removing a wall, add it to neighbour lists
		v.push_back(pointToIndex(t_point));
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
			if (!t_all && !m_isTraversible[pointToIndex({ _row, _col })]) continue;

			t_vec.push_back(pointToIndex({ _row, _col }));
		}
	}
}
