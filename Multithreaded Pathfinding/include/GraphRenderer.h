#ifndef GRAPH_RENDERER_H
#define GRAPH_RENDERER_H

#include <SFML/Graphics.hpp>

#include "Graph.h"

class GraphRenderer
{
public:
	/// <summary>
	/// C'tor taking a size (pixels) and a Graph
	/// </summary>
	/// <param name="t_size">The size of our graph on-screen on pixels</param>
	/// <param name="t_graph">A pointer to our graph</param>
	GraphRenderer(sf::Vector2f t_size, Graph const* t_graph) :
		m_size(t_size), m_graph(t_graph) {
		m_cellSize = sf::Vector2f(
			m_size.x / m_graph->COLS, // Width / cols
			m_size.y / m_graph->ROWS // Height / rows
		);
		initializeGrid();
	};

	/// <summary>
	/// Set the graph for us to draw
	/// </summary>
	/// <param name="t_graph">A pointer to our graph</param>
	void setGraph(Graph const* t_graph) { m_graph = t_graph; }

	~GraphRenderer() { m_graph = nullptr; }

	/// <summary>
	/// Sets the colour of a particular cell
	/// </summary>
	/// <param name="t_index">Index of the cell to set the colour for</param>
	/// <param name="t_color">sf::Color to set cell to</param>
	void setColor(int t_index, sf::Color t_color);

	void update();

	void draw(sf::RenderWindow& t_window);

private:
	void initializeGrid();

	/// <summary>
	/// Returns sf::Vertex[4] representing a cell drawn at a given
	/// position in a given colour, intended for insertion into a vBuffer
	/// </summary>
	/// <param name="t_position">Top-left position of cell</param>
	/// <param name="t_color">Colour of the cell</param>
	/// <returns>Pointer to the first of 4 vertices in contiguous memory</returns>
	void createCell(sf::Vector2f t_position, sf::Color t_color, sf::Vertex* t_vertices);

	// The dimensions of our cell in x,y coordinates
	sf::Vector2f m_cellSize;

	Graph const* m_graph{ nullptr };

	// The size of our graph on-screen in pixels
	sf::Vector2f m_size;

	sf::VertexBuffer m_vBuffer{ sf::Quads, sf::VertexBuffer::Usage::Dynamic };
};

#endif