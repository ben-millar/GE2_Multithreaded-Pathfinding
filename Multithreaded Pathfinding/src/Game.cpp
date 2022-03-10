#include "Game.h"

void Game::run()
{
	m_window = createWindow("GE2 | Multithreaded Pathfinding");
	m_window->setKeyRepeatEnabled(false);
	m_window->setView({ {960.f, 540.f}, { 1920.f, 1080.f } });

	loadTextures();

	auto sceneManager = SceneManager::getInstance();
	sceneManager->setWindow(m_window);

	sceneManager->registerScene<GameplayScene>(SceneType::GAMEPLAY);
	sceneManager->registerScene<MainMenuScene>(SceneType::MAIN_MENU);

	sceneManager->setScene(SceneType::GAMEPLAY);

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

	tm->loadTexture("30_grid", "assets/images/30_grid.png");
	tm->loadTexture("30_player", "assets/images/30_player.png");
	tm->loadTexture("30_npc", "assets/images/30_npc.png");
	tm->loadTexture("30_wall", "assets/images/30_wall.png");

	tm->loadTexture("100_grid", "assets/images/100_grid.png");
	tm->loadTexture("100_player", "assets/images/100_player.png");
	tm->loadTexture("100_npc", "assets/images/100_npc.png");
}

////////////////////////////////////////////////////////////

sf::RenderWindow* Game::createWindow(std::string t_title)
{
	return new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), t_title, sf::Style::Titlebar | sf::Style::Close);
}
