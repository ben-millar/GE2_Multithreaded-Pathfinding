#include "Graph.h"

Graph::Graph(int rows, int cols) :
	ROWS(rows),
	COLS(cols)
{
	m_neighbours = new std::vector<int>[ROWS * COLS];

	m_distance = new float[ROWS * COLS];
	std::fill_n(m_distance, ROWS * COLS, 0.f);

	m_cost = new int[ROWS * COLS];
	std::fill_n(m_cost, ROWS * COLS, 0);

	m_heuristic = new float[ROWS * COLS];
	std::fill_n(m_heuristic, ROWS * COLS, 0.f);

	m_previous = new int[ROWS * COLS];
	std::fill_n(m_previous, ROWS * COLS, -1);

	m_visited = new bool[ROWS * COLS];
	std::fill_n(m_visited, ROWS * COLS, false);

	m_isTraversible = new bool[ROWS * COLS];
	std::fill_n(m_isTraversible, ROWS * COLS, true);

	generateGraph();
}

////////////////////////////////////////////////////////////

Graph::~Graph()
{
	delete[] m_neighbours;
	delete[] m_distance;
	delete[] m_cost;
	delete[] m_heuristic;
	delete[] m_previous;
	delete[] m_visited;
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

Graph::Path Graph::findPath(Point t_origin, Point t_destination)
{
	auto cmp = [&](int i1, int i2) {
		return (m_heuristic[i1] + m_distance[i1]) >
			   (m_heuristic[i2] + m_distance[i2]);
	};

	// Get the indices of our start and end points
	size_t start = pointToIndex({ t_origin.x, t_origin.y });
	size_t goal = pointToIndex({ t_destination.x, t_destination.y });

	// Reset our data
	std::fill_n(m_distance, ROWS * COLS, std::numeric_limits<float>::max());
	std::fill_n(m_visited, ROWS * COLS, false);
	std::fill_n(m_previous, ROWS * COLS, -1);

	for (int i = 0; i < (ROWS * COLS); ++i)
		m_heuristic[i] = distanceBetweenPoints(i, goal);

	std::priority_queue<int, std::vector<int>, decltype(cmp)> pQ(cmp);

	m_visited[start] = true;
	m_distance[start] = 0.f;

	pQ.push(start);

	// While our queue isn't empty, and we haven't found our goal
	while (!pQ.empty() && goal != pQ.top())
	{
		int current = pQ.top();
		pQ.pop();

		// Iterate through the arcs on the current node
		for (auto& neighbour : m_neighbours[current])
		{
			// If we're not returning to the previous node
			if (m_previous[neighbour] == current)
				continue;

			// If this is shorter than the previous best path
			float value = m_distance[current] + m_cost[neighbour];
			if (m_distance[neighbour] > value)
			{
				// Update the path cost to this shorter path
				m_distance[neighbour] = m_distance[current];
				m_previous[neighbour] = current;
			}

			// If this node isn't visited
			if (!m_visited[neighbour])
			{
				// Mark it and add it to the queue
				m_visited[neighbour] = true;
				pQ.push(neighbour);
			}
		}
	}

	std::stack<int> st;
	for (int n = goal; -1 != n && n != start; n = m_previous[n])
		st.push(n);

	st.push(start);

	return st;
}

////////////////////////////////////////////////////////////

float Graph::distanceBetweenPoints(int t_origin, int t_destination)
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
