#include "Graph.h"

Graph::Graph(int t_rows, int t_cols) :
	ROWS(t_rows),
	COLS(t_cols)
{
	m_neighbours = new std::vector<int>[ROWS * COLS];

	m_isTraversible = new bool[ROWS * COLS];
	std::fill_n(m_isTraversible, ROWS * COLS, true);

	generateGraph();
}

////////////////////////////////////////////////////////////

Graph::~Graph()
{
	delete[] m_neighbours;
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

float Graph::distanceBetweenPoints(int t_origin, int t_destination) const
{
	auto coords = [&](int index) {
		return Point(index % COLS, index / ROWS);
	};

	Point deltaPos = coords(t_destination) - coords(t_origin);

	return sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);
}

////////////////////////////////////////////////////////////

void Graph::neighbours(int t_row, int t_col, std::vector<int>& t_vec)
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
			if (!m_isTraversible[pointToIndex({ _row, _col })]) continue;

			t_vec.push_back(pointToIndex({ _row, _col }));
		}
	}
}
