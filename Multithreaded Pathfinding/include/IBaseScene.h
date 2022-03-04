#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include <SFML/Graphics.hpp>

class IBaseScene
{
public:
	IBaseScene() = default;
	virtual ~IBaseScene() = default;

	/// <summary>
	/// Sets the render window which should be used by the scene
	/// </summary>
	/// <param name="t_window"></param>
	virtual void setWindow(sf::RenderWindow* t_window) { m_window = t_window; }

	/// <summary>
	/// Handles system events (input, etc.)
	/// </summary>
	virtual void processEvents() = 0;

	/// <summary>
	/// Called once every game tick
	/// </summary>
	/// <param name="t_dTime">Time in seconds since the last update</param>
	virtual void update(sf::Time t_dT) = 0;

	/// <summary>
	/// Draw to our game window
	/// </summary>
	virtual void render() = 0;

protected:
	sf::RenderWindow* m_window;
};

#endif