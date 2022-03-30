#include "Graph.h"

Graph::Graph(int t_rows, int t_cols) :
	ROWS(t_rows),
	COLS(t_cols)
{
	nodes = new Node[ROWS * COLS];
	std::fill_n(nodes, ROWS * COLS, Node());

	generateGraph();
}

////////////////////////////////////////////////////////////

Graph::~Graph()
{
	delete[] nodes;
}

////////////////////////////////////////////////////////////

void Graph::generateGraph()
{
	for (int row = 0; row < ROWS; ++row)
	{

		for (int col = 0; col < COLS; ++col)
		{

		}
	}
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
