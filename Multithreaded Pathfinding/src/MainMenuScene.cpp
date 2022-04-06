#include "MainMenuScene.h"

MainMenuScene::MainMenuScene()
{
	DEBUG_INFO("Creating " << typeid(*this).name());
	m_bgSprite.setTexture(*TextureManager::getInstance()->getTexture("background"));
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
			case sf::Keyboard::Num1:
				SceneManager::getInstance()->setScene(SceneType::GAMEPLAY_30);
				return;
			case sf::Keyboard::Num2:
				SceneManager::getInstance()->setScene(SceneType::GAMEPLAY_100);
				return;
			case sf::Keyboard::Num3:
				SceneManager::getInstance()->setScene(SceneType::GAMEPLAY_1000);
				return;
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

	m_window->draw(m_bgSprite);

	m_window->display();
}
