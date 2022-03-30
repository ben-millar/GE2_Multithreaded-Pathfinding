#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());

	m_graph = new Graph(30, 30);
	m_pathfinder = new Pathfinder(30, 30);

	TextureManager* tm = TextureManager::getInstance();

	m_background.setTexture(*tm->getTexture("30_grid"));
	m_background.setPosition({ 460.f, 0.f });

	m_player.setTexture(*tm->getTexture("30_player"));
	m_player.setPosition({ -100.f,-100.f });
}

////////////////////////////////////////////////////////////

GameplayScene::~GameplayScene()
{
	DEBUG_INFO("Destroying " << typeid(*this).name());

	delete m_graph;
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
				m_window->close();
				break;
			case sf::Keyboard::P:
				findPath();
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
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
			sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

			if (m_background.getGlobalBounds().contains(worldPos))
			{
				switch (e.mouseButton.button)
				{
				case sf::Mouse::Left:
					placeNPC(worldPos);
					break;
				case sf::Mouse::Middle:
					m_graph->setWall(mouseClickToPoint(worldPos));
					placeWall(worldPos);
					break;
				case sf::Mouse::Right:
					placePlayer(worldPos);
					break;
				default:
					break;
				}
			}
		}

		if (e.type == sf::Event::MouseWheelMoved)
		{	
			//sf::View v = m_window->getView();

			//v.zoom(1 - e.mouseWheel.delta * 0.05f);

			//m_window->setView(v);
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
	static const sf::Vector2f BOARD_OFFSET = { 460.f, 0.f };

	sf::Vector2f playerPos = m_player.getPosition() - BOARD_OFFSET;
	sf::Vector2f npcPos = m_NPCs.at(0).getPosition() - BOARD_OFFSET;
	
	Point player = {
		(int)(playerPos.y / CELL_WIDTH),
		(int)(playerPos.x / CELL_HEIGHT)
	};

	Point npc = {
		(int)(npcPos.y / CELL_WIDTH),
		(int)(npcPos.x / CELL_HEIGHT)
	};

	Pathfinder::Path path = m_pathfinder->findPath(npc, player, m_graph);

	while (!path.empty())
	{
		int index = path.top();
		path.pop();

		int row = index / COLS;
		int col = index % COLS;

		DEBUG_INFO(row << ", " << col);
	}
}

////////////////////////////////////////////////////////////

Point GameplayScene::mouseClickToPoint(sf::Vector2f t_mousePos)
{
	sf::Vector2f localPos = t_mousePos - BOARD_OFFSET;

	int row = (int)(localPos.y / CELL_WIDTH);
	int col = (int)(localPos.x / CELL_HEIGHT);

	return Point(row, col);
}

////////////////////////////////////////////////////////////

void GameplayScene::placePlayer(sf::Vector2f t_position)
{
	Point p = mouseClickToPoint(t_position);

	sf::Vector2f pos = { (float)(p.col * CELL_WIDTH), (float)(p.row * CELL_HEIGHT) };
	pos += BOARD_OFFSET;

	m_player.setPosition(pos);
}

////////////////////////////////////////////////////////////

void GameplayScene::placeNPC(sf::Vector2f t_position)
{
	static sf::Texture* tx = TextureManager::getInstance()->getTexture("30_npc");
	sf::Sprite spr;
	spr.setTexture(*tx);

	Point p = mouseClickToPoint(t_position);

	sf::Vector2f pos = { (float)(p.col * CELL_WIDTH), (float)(p.row * CELL_HEIGHT) };
	pos += BOARD_OFFSET;

	spr.setPosition(pos);
	m_NPCs.push_back(spr);
}

////////////////////////////////////////////////////////////

void GameplayScene::placeWall(sf::Vector2f t_position)
{
	static sf::Texture* tx = TextureManager::getInstance()->getTexture("30_wall");
	sf::Sprite spr;
	spr.setTexture(*tx);

	Point p = mouseClickToPoint(t_position);

	sf::Vector2f pos = { (float)(p.col * CELL_WIDTH), (float)(p.row * CELL_HEIGHT) };
	pos += BOARD_OFFSET;

	spr.setPosition(pos);
	m_walls.push_back(spr);
}

////////////////////////////////////////////////////////////

void GameplayScene::render()
{
	m_window->clear(sf::Color::Black);

	m_window->draw(m_background);
	m_window->draw(m_player);

	for (auto& spr : m_NPCs)
		m_window->draw(spr);

	for (auto& spr : m_walls)
		m_window->draw(spr);

	m_window->display();
}
