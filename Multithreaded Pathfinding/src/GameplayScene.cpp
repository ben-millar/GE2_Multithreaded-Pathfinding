#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());

	// Leave one thread available for the main thread
	m_threadPool = new thread_pool(std::thread::hardware_concurrency() - 1);

	m_graph = new Graph(100, 100);
	m_graphRenderer = new GraphRenderer({ 1080,1080 }, m_graph);

	const int NUM_THREADS = m_threadPool->get_thread_count();
	m_pathfinders = new Pathfinder * [NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; ++i)
		m_pathfinders[i] = new Pathfinder(100, 100);

	for (int i = 0; i < 50; ++i)
	{
		int index = (rand() % 100) * (rand() % 100);
		m_NPCs.push_back(index);
		
	}

	m_graphRenderer->updateNPCs(&m_NPCs);
}

////////////////////////////////////////////////////////////

GameplayScene::~GameplayScene()
{
	DEBUG_INFO("Destroying " << typeid(*this).name());

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
	m_graph->resetCosts();
	/// <summary>
	/// Single-thread benchmark
	/// </summary>
	//auto t1 = high_resolution_clock::now();

	//for (int const& npc : m_NPCs)
	//{
	//	Pathfinder::Path path = m_pathfinder->findPath(npc, m_player, m_graph);

	//	while (!path.empty())
	//	{
	//		int index = path.top();
	//		path.pop();
	//		m_graphRenderer->setColor(index, sf::Color::Yellow);
	//	}
	//}

	//auto t2 = high_resolution_clock::now();
	//std::cout << "Single thread: " << duration_cast<milliseconds>(t2 - t1).count() << std::endl;
	/// <summary>
	/// Multithreading benchmark
	/// </summary>

	static const int NUM_THREADS = m_threadPool->get_thread_count();

	int nextAvailable = 0;

	std::vector<std::future<Pathfinder::Path>> results;
	results.reserve(m_NPCs.size());

	auto t3 = high_resolution_clock::now();

	for (int const& npc : m_NPCs)
	{
		results.push_back(m_threadPool->submit([&]() {
			return m_pathfinders[nextAvailable++ % NUM_THREADS]->findPath(npc, m_player, m_graph);
			}
		));
	}

	while (true)
	{
		std::this_thread::sleep_for(1000ms);
		int remaining = m_threadPool->get_tasks_total();

		std::cout.flush();
		std::cout << "Paths complete: [" << 50-remaining << "/50]" << std::endl;

		if (!remaining) break;
	}

	m_threadPool->wait_for_tasks();

	for (auto& result : results)
	{
		drawPath(result.get());
	}

	auto t4 = high_resolution_clock::now();
	std::cout << "Multithreaded: " << duration_cast<milliseconds>(t4 - t3).count() << std::endl;
}

////////////////////////////////////////////////////////////

void GameplayScene::drawPath(Pathfinder::Path t_path)
{
	sf::Color color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	while (!t_path.empty())
	{
		int index = t_path.top();
		t_path.pop();
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
