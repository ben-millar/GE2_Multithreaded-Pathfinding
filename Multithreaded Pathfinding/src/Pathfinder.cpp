#include "Pathfinder.h"

Pathfinder::Pathfinder(int t_rows, int t_cols) :
	ROWS{ t_rows }, COLS{ t_cols }
{
	m_distance = new float[ROWS * COLS];
	std::fill_n(m_distance, ROWS * COLS, 0.f);

	m_heuristic = new float[ROWS * COLS];
	std::fill_n(m_heuristic, ROWS * COLS, 0.f);

	m_previous = new int[ROWS * COLS];
	std::fill_n(m_previous, ROWS * COLS, -1);

	m_visited = new bool[ROWS * COLS];
	std::fill_n(m_visited, ROWS * COLS, false);
}

////////////////////////////////////////////////////////////

Pathfinder::~Pathfinder()
{
	delete[] m_distance;
	delete[] m_heuristic;
	delete[] m_previous;
	delete[] m_visited;
}

////////////////////////////////////////////////////////////

void Pathfinder::findPath(int t_origin, int t_destination, std::shared_ptr<Path> t_path, Graph* t_graph)
{
	const std::scoped_lock lock(m_mtx);

	auto cmp = [&](int i1, int i2) {
		return (m_heuristic[i1] + m_distance[i1]) >
			(m_heuristic[i2] + m_distance[i2]);
	};

	if (t_path.get()->size() < 0)
		std::cout << "AHHHHHHHHHHHHHHHHHHHH";

	// Get the indices of our start and end points
	size_t start = t_origin;
	size_t goal = t_destination;

	// Get our list of neighbours
	std::vector<int> const* neighbours = t_graph->getNeighbours();
	int const* pathCost = t_graph->getCost();

	// Reset our data
	std::fill_n(m_distance, ROWS * COLS, std::numeric_limits<float>::max());
	std::fill_n(m_visited, ROWS * COLS, false);
	std::fill_n(m_previous, ROWS * COLS, -1);

	for (int i = 0; i < (ROWS * COLS); ++i)
		m_heuristic[i] = t_graph->distanceBetweenIndices(i, goal);

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
		for (auto neighbour : neighbours[current])
		{
			// If we're not returning to the previous node
			if (m_previous[neighbour] == current)
				continue;

			// If this is shorter than the previous best path
			float value = m_distance[current] + pathCost[neighbour];

			if (m_distance[neighbour] > value)
			{
				// Update the path cost to this shorter path
				m_distance[neighbour] = value;
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

	for (int n = goal; -1 != n && n != start; n = m_previous[n])
	{
		t_graph->increaseCost(n);
		t_path->push(n);
	}
}
