#include "Game.h"

void Game::run()
{
	m_window = createWindow("GE2 | Multithreaded Pathfinding");
	m_window->setKeyRepeatEnabled(false);
	m_window->setView({ {540.f, 540.f}, { 1080.f, 1080.f } });

	loadTextures();

	auto sceneManager = SceneManager::getInstance();
	sceneManager->setWindow(m_window);

	sceneManager->registerScene<GameplayScene>(SceneType::GAMEPLAY_30, 30, 5);
	sceneManager->registerScene<GameplayScene>(SceneType::GAMEPLAY_100, 100, 50);
	sceneManager->registerScene<GameplayScene>(SceneType::GAMEPLAY_1000, 1000, 500);
	sceneManager->registerScene<MainMenuScene>(SceneType::MAIN_MENU);

	sceneManager->setScene(SceneType::MAIN_MENU);

	sf::Clock clock;
	sf::Time lag = sf::Time::Zero;
	const sf::Time MS_PER_UPDATE = sf::seconds(1 / 60.0f);

	while (m_window->isOpen())
	{
		sf::Time dT = clock.restart();
		lag += dT;

		sceneManager->processEvents();

		while (lag > MS_PER_UPDATE)
		{
			sceneManager->update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		sceneManager->update(dT);
		sceneManager->render();
	}
}

////////////////////////////////////////////////////////////

void Game::loadTextures()
{
	TextureManager* tm = TextureManager::getInstance();
}

////////////////////////////////////////////////////////////

sf::RenderWindow* Game::createWindow(std::string t_title)
{
	return new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), t_title, sf::Style::Titlebar | sf::Style::Close);
}
