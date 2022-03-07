#include "Graph.h"

Graph::Graph(int rows, int cols) :
	ROWS(rows),
	COLS(cols)
{
	m_neighbours = new std::vector<int>[ROWS * COLS];
	m_heuristic = new int[ROWS * COLS];
	m_distance = new int[ROWS * COLS];
	m_isTraversible = new bool[ROWS * COLS];

	std::fill_n(m_heuristic, ROWS * COLS, 0);
	std::fill_n(m_distance, ROWS * COLS, 0);
	std::fill_n(m_isTraversible, ROWS * COLS, true);

	generateGraph();
}

////////////////////////////////////////////////////////////

Graph::~Graph()
{
	delete[] m_neighbours;
	delete[] m_heuristic;
	delete[] m_distance;
	delete[] m_isTraversible;
}

////////////////////////////////////////////////////////////

void Graph::generateGraph()
{
	auto toIndex = [&](int row, int col) {
		return row * COLS + col;
	};

	for (int row = 0; row < COLS; ++row)
	{
		for (int col = 0; col < ROWS; ++col)
		{
			neighbours(row, col, m_neighbours[toIndex(row, col)]);

			m_heuristic[toIndex(row, col)] = 1;
			m_distance[toIndex(row, col)] = 1;
		}
	}
}

////////////////////////////////////////////////////////////

void Graph::neighbours(int row, int col, std::vector<int>& vec)
{
	auto toIndex = [&](int row, int col) {
		return row * COLS + col;
	};

	int _row, _col;

	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			_row = row + i;
			_col = col + j;

			if (_row == row && _col == col) continue;
			if (_row < 0 || _row >= ROWS) continue;
			if (_col < 0 || _col >= COLS) break;
			if (!m_isTraversible[toIndex(_row, _col)]) continue;

			vec.push_back(toIndex(_row, _col));
		}
	}
}
