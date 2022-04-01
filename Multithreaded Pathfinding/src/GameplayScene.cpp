#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());

	m_graph = new Graph(30, 30);
	m_graphRenderer = new GraphRenderer({ 1080,1080 }, m_graph);
	m_pathfinder = new Pathfinder(30, 30);
}

////////////////////////////////////////////////////////////

GameplayScene::~GameplayScene()
{
	DEBUG_INFO("Destroying " << typeid(*this).name());

	delete m_graph;
	delete m_graphRenderer;
	delete m_pathfinder;
}

////////////////////////////////////////////////////////////

void GameplayScene::processEvents()
{
	sf::Event e;

	while (m_window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_window->close();

		if (e.type == sf::Event::KeyPressed)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::Escape:
				SceneManager::getInstance()->setScene(SceneType::MAIN_MENU);
				return;
				break;
			case sf::Keyboard::Space:
				findPath();
				break;
			default:
				break;
			}
		}

		if (e.type == sf::Event::MouseButtonReleased)
		{
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
			sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

			if (m_screenBounds.contains(worldPos))
			{
				int index = mouseClickToIndex(worldPos);

				switch (e.mouseButton.button)
				{
				case sf::Mouse::Left:
					m_player = index;
					m_graphRenderer->setPlayerIndex(m_player);
					break;
				case sf::Mouse::Middle:
					m_graph->toggleWall(index);
					m_graphRenderer->toggleWall(index);
					break;
				case sf::Mouse::Right:
					m_NPCs.push_back(index);
					m_graphRenderer->updateNPCs(&m_NPCs);
					break;
				default:
					break;
				}
			}
		}

		if (e.type == sf::Event::MouseWheelMoved)
		{	
			sf::View v = m_window->getView();
			v.zoom(1 - e.mouseWheel.delta * 0.05f);

			if (v.getSize().x > m_screenBounds.width)
				v.setSize({ m_screenBounds.width, m_screenBounds.height });

			m_window->setView(v);
		}

		if (e.type == sf::Event::MouseMoved)
		{
			//sf::View v = m_window->getView();

			//sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
			//sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

			//v.setCenter(worldPos);

			//m_window->setView(v);
		}
	}
}

////////////////////////////////////////////////////////////

void GameplayScene::update(sf::Time t_dT)
{
}

////////////////////////////////////////////////////////////

void GameplayScene::findPath()
{
	for (int const& npc : m_NPCs)
	{
		Pathfinder::Path path = m_pathfinder->findPath(npc, m_player, m_graph);

		while (!path.empty())
		{
			int index = path.top();
			path.pop();
			m_graphRenderer->setColor(index, sf::Color::Yellow);

			int row = index / m_graph->COLS;
			int col = index % m_graph->ROWS;
		}
	}
}

////////////////////////////////////////////////////////////

int GameplayScene::mouseClickToIndex(sf::Vector2f t_mousePos)
{
	int row = t_mousePos.y / (m_screenBounds.width / m_graph->ROWS);
	int col = t_mousePos.x / (m_screenBounds.height / m_graph->COLS);

	return m_graph->pointToIndex({row, col});
}

////////////////////////////////////////////////////////////

void GameplayScene::render()
{
	m_window->clear(sf::Color::Black);

	m_graphRenderer->draw(*m_window);

	m_window->display();
}
