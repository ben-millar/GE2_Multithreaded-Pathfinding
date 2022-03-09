#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());

	m_graph = new Graph(10, 10);
	Graph::Path path = m_graph->findPath({ 0,0 }, { 9,9 });

	while (!path.empty())
		DEBUG_INFO(path.top()), path.pop();

	TextureManager* tm = TextureManager::getInstance();

	m_background.setTexture(*tm->getTexture("100_grid"));
	m_background.setPosition({ 460.f, 0.f });

	m_player.setTexture(*tm->getTexture("100_player"));
	m_player.setPosition({ -100.f,-100.f });
}

////////////////////////////////////////////////////////////

GameplayScene::~GameplayScene()
{
	DEBUG_INFO("Destroying " << typeid(*this).name());
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
				m_window->close();
				break;
			//case sf::Keyboard::Space:
			//	SceneManager::getInstance()->setScene(SceneType::MAIN_MENU);
			//	return;
			default:
				break;
			}
		}

		if (e.type == sf::Event::MouseButtonReleased)
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

				placeNPC(worldPos);
			}
			else if (e.mouseButton.button == sf::Mouse::Right)
			{
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

				placePlayer(worldPos);
			}
		}

		if (e.type == sf::Event::MouseWheelMoved)
		{	
			sf::View v = m_window->getView();

			v.zoom(1 - e.mouseWheel.delta * 0.05f);

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

void GameplayScene::placePlayer(sf::Vector2f t_position)
{
	static const sf::Vector2f OFFSET = { 460.f, 0.f };

	sf::Vector2f worldPos = t_position;
	sf::Vector2f localPos = worldPos - OFFSET;

	static const sf::Vector2i GRID_DIMENSIONS = { 1000, 1000 };
	static const int ROWS = 100;
	static const int COLS = 100;
	static const int CELL_WIDTH = GRID_DIMENSIONS.x / COLS;
	static const int CELL_HEIGHT = GRID_DIMENSIONS.y / ROWS;

	int row = localPos.y / CELL_WIDTH;
	int col = localPos.x / CELL_HEIGHT;

	sf::Vector2f pos = { (float)(col * CELL_WIDTH), (float)(row * CELL_HEIGHT) };
	pos += OFFSET;

	m_player.setPosition(pos);
}

////////////////////////////////////////////////////////////

void GameplayScene::placeNPC(sf::Vector2f t_position)
{
	static const sf::Vector2f OFFSET = { 460.f, 0.f };

	sf::Vector2f worldPos = t_position;
	sf::Vector2f localPos = worldPos - OFFSET;

	static const sf::Vector2i GRID_DIMENSIONS = { 1000, 1000 };
	static const int ROWS = 100;
	static const int COLS = 100;
	static const int CELL_WIDTH = GRID_DIMENSIONS.x / COLS;
	static const int CELL_HEIGHT = GRID_DIMENSIONS.y / ROWS;

	static sf::Texture* tx = TextureManager::getInstance()->getTexture("100_npc");
	sf::Sprite spr;
	spr.setTexture(*tx);
	
	int row = localPos.y / CELL_WIDTH;
	int col = localPos.x / CELL_HEIGHT;

	sf::Vector2f pos = { (float)(col * CELL_WIDTH), (float)(row * CELL_HEIGHT) };
	pos += OFFSET;

	spr.setPosition(pos);
	m_NPCs.push_back(spr);
}

////////////////////////////////////////////////////////////

void GameplayScene::render()
{
	m_window->clear(sf::Color::Black);

	m_window->draw(m_background);
	m_window->draw(m_player);

	for (auto& spr : m_NPCs)
		m_window->draw(spr);

	m_window->display();
}
