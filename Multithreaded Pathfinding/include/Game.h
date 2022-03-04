#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Globals.h"

#include "SceneManager.h"
#include "GameplayScene.h"
#include "MainMenuScene.h"

using namespace std;

class Game
{
public:
	Game() = default;
	~Game() = default;

	/// <summary>
	/// Initializes and starts the game loop
	/// </summary>
	void run();

private:
	/// <summary>
	/// Creates an sf::RenderWindow on the stack
	/// </summary>
	/// <param name="t_title">Title for the sf::RenderWindow</param>
	/// <returns>A pointer to a stack-allocated sf::RenderWindow</returns>
	sf::RenderWindow* createWindow(std::string t_title);
	sf::RenderWindow* m_window{ nullptr };
};

#endif