#include "GraphRenderer.h"

void GraphRenderer::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_vBuffer);
}

////////////////////////////////////////////////////////////

void GraphRenderer::initializeGrid()
{
	static sf::Color colors[2] = {
		sf::Color::White,
		sf::Color(191,191,191,255)
	};

	// Create our buffer with 4 vertices per cell
	m_vBuffer.create(4 + (m_graph->ROWS * m_graph->COLS * 4));
	sf::Vertex vertices[4];

	int offset{ 0 };

	for (int row = 0; row < m_graph->ROWS; ++row)
	{
		for (int col = 0; col < m_graph->COLS; ++col)
		{
			sf::Vector2f position{ m_cellSize.x * col, m_cellSize.y * row };
			createCell(position, colors[(row+col)%2], vertices);
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

	sf::Vector2f position{ m_cellSize.x * col, m_cellSize.y * row };

	createCell(position, t_color, vertices);

	m_vBuffer.update(vertices, 4, offset);
}

////////////////////////////////////////////////////////////

void GraphRenderer::update()
{
	bool const* walls = m_graph->getWalls();

	for (int i = 0; i < m_graph->ROWS * m_graph->COLS; ++i)
		if (!walls[i]) setColor(i, sf::Color::Black);
}
