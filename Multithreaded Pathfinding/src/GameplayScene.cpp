#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());

	Graph* graph = new Graph(1000, 1000);
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
	}
}

////////////////////////////////////////////////////////////

void GameplayScene::update(sf::Time t_dT)
{
}

////////////////////////////////////////////////////////////

void GameplayScene::render()
{
	m_window->clear(sf::Color::Transparent);

	m_window->display();
}
