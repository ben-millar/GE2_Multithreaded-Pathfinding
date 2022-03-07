#include "MainMenuScene.h"

MainMenuScene::MainMenuScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());
}

////////////////////////////////////////////////////////////

MainMenuScene::~MainMenuScene()
{
	DEBUG_INFO("Destroying " << typeid(*this).name());
}

////////////////////////////////////////////////////////////

void MainMenuScene::processEvents()
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
			//	SceneManager::getInstance()->setScene(SceneType::GAMEPLAY);
			//	return;
			default:
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////

void MainMenuScene::update(sf::Time t_dT)
{
}

////////////////////////////////////////////////////////////

void MainMenuScene::render()
{
	m_window->clear(sf::Color::Transparent);

	m_window->display();
}
