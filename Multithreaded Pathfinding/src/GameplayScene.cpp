#include "GameplayScene.h"

GameplayScene::GameplayScene(int t_gridSize, int t_numNPCs) :
	GRID_SIZE(t_gridSize), NUM_NPCS(t_numNPCs)
{
	DEBUG_INFO("Creating " << typeid(*this).name());

	// Leave one thread available for the main thread
	m_threadPool = new ThreadPool(std::thread::hardware_concurrency() - 1);

	m_graph = new Graph(GRID_SIZE, GRID_SIZE);
	m_graphRenderer = new GraphRenderer({ 1080,1080 }, m_graph);

	const int NUM_THREADS = m_threadPool->getThreadCount();
	m_pathfinders = new Pathfinder * [NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; ++i)
		m_pathfinders[i] = new Pathfinder(GRID_SIZE, GRID_SIZE);

	for (int i = 0; i < NUM_NPCS; ++i)
	{
		int index = (rand() % GRID_SIZE) * (rand() % GRID_SIZE);
		m_NPCs.push_back(index);	
	}

	m_graphRenderer->updateNPCs(&m_NPCs);
}

////////////////////////////////////////////////////////////

GameplayScene::~GameplayScene()
{
	DEBUG_INFO("Destroying " << typeid(*this).name());

	delete m_threadPool;
	delete m_graph;
	delete m_graphRenderer;
	delete[] m_pathfinders;
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
				m_threadPool->submit([&]() {findPath(); });
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
	}
}

////////////////////////////////////////////////////////////

void GameplayScene::update(sf::Time t_dT)
{
	if (m_window->hasFocus())
		scrollView(t_dT);
}

////////////////////////////////////////////////////////////

void GameplayScene::findPath()
{
	m_graph->resetCosts();

	static const int NUM_THREADS = m_threadPool->getThreadCount();

	int nextAvailable = 0;

	std::queue<std::future<bool>> futures;
	std::queue<std::shared_ptr<Pathfinder::Path>> results;

	auto t3 = high_resolution_clock::now();

	for (int const& npc : m_NPCs)
	{
		auto p = std::make_shared<Pathfinder::Path>();

		futures.push(m_threadPool->submit([this, npc, p, nextAvailable]() {
			m_pathfinders[nextAvailable % NUM_THREADS]->findPath(npc, m_player, p, m_graph);
			}));

		results.push(p);

		nextAvailable++;
	}

	while (!futures.empty() && !results.empty())
	{
		futures.front().wait();
		futures.pop();

		drawPath(results.front());
		results.pop();
	}

	auto t4 = high_resolution_clock::now();
	DEBUG_INFO("Multithreaded: " << duration_cast<milliseconds>(t4 - t3).count());
}

////////////////////////////////////////////////////////////

void GameplayScene::drawPath(std::shared_ptr<Pathfinder::Path> t_path)
{
	sf::Color color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	while (!t_path->empty())
	{
		int index = t_path->top();
		t_path->pop();
		m_graphRenderer->setColor(index, color);
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

////////////////////////////////////////////////////////////

void GameplayScene::scrollView(sf::Time t_dT)
{
	sf::View v = m_window->getView();
	float zoom = RESOLUTION.x / v.getSize().x;
	sf::Vector2f center = v.getCenter();

	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
	sf::Vector2f worldPos = m_window->mapPixelToCoords(mousePos);

	sf::Vector2f delta = center - worldPos;
	float len = sqrt(delta.x * delta.x + delta.y * delta.y);

	if (len > 200.f / zoom) {
		sf::Vector2f newPos = (center * 0.999f) + (worldPos * 0.001f);
		newPos.x = std::clamp(newPos.x, 540.f / zoom, 1080.f - (540.f / zoom));
		newPos.y = std::clamp(newPos.y, 540.f / zoom, 1080.f - (540.f / zoom));

		v.setCenter(newPos);

		m_window->setView(v);
	}
}
