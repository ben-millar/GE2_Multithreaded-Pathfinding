#include "GraphRenderer.h"

void GraphRenderer::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_vBuffer);
}

////////////////////////////////////////////////////////////

void GraphRenderer::initializeGrid()
{
	// Create our buffer with 4 vertices per cell
	m_vBuffer.create(4 + (m_graph->ROWS * m_graph->COLS * 4));
	sf::Vertex vertices[4];

	int offset{ 0 };

	for (int row = 0; row < m_graph->ROWS; ++row)
	{
		for (int col = 0; col < m_graph->COLS; ++col)
		{
			sf::Vector2f position{ m_cellSize.x * col, m_cellSize.y * row };
			createCell(position, m_backgroundColors[(row+col)%2], vertices);
			m_vBuffer.update(vertices, 4, offset+=4);
		}
	}
}

////////////////////////////////////////////////////////////

void GraphRenderer::createCell(sf::Vector2f t_position, sf::Color t_color, sf::Vertex* t_vertices)
{
	t_vertices[0] = sf::Vertex(t_position, t_color);
	t_vertices[1] = sf::Vertex(t_position + sf::Vector2f(m_cellSize.x, 0.f), t_color);
	t_vertices[2] = sf::Vertex(t_position + sf::Vector2f(m_cellSize.x, m_cellSize.y), t_color);
	t_vertices[3] = sf::Vertex(t_position + sf::Vector2f(0.f, m_cellSize.y), t_color);
}

////////////////////////////////////////////////////////////

void GraphRenderer::setColor(int t_index, sf::Color t_color)
{
	sf::Vertex vertices[4];

	int row = t_index / m_graph->COLS;
	int col = t_index % m_graph->ROWS;
	int offset = 4 + (t_index * 4);

	if (sf::Color::Transparent == t_color)
		t_color = m_backgroundColors[(row + col) % 2];

	sf::Vector2f position{ m_cellSize.x * col, m_cellSize.y * row };

	createCell(position, t_color, vertices);

	m_vBuffer.update(vertices, 4, offset);
}

////////////////////////////////////////////////////////////

void GraphRenderer::toggleWall(int t_index)
{
	if (m_walls.count(t_index))
	{
		m_walls.erase(t_index);
		setColor(t_index);
	}
	else
	{
		m_walls.insert(t_index);
		setColor(t_index, sf::Color::Black);
	}
}

////////////////////////////////////////////////////////////

void GraphRenderer::updateNPCs(std::vector<int>* t_vec)
{
	for (int const& index : m_NPCpositions)
		setColor(index);

	std::copy(t_vec->begin(), t_vec->end(), std::back_inserter(m_NPCpositions));

	for (int const& index : m_NPCpositions)
		setColor(index, sf::Color::Red);
}

////////////////////////////////////////////////////////////

void GraphRenderer::setPlayerIndex(int t_index)
{
	setColor(m_playerPos);
	m_playerPos = t_index;
	setColor(m_playerPos, sf::Color::Green);
}
