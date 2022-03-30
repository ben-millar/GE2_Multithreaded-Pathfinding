#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "IBaseScene.h"
#include "Globals.h"
#include "Graph.h"
#include "Pathfinder.h"
#include "SceneManager.h"
#include "TextureManager.h"

class GameplayScene :
    public IBaseScene
{
public:
    GameplayScene();
    ~GameplayScene();

    /// <summary>
    /// Handles system events (input, etc.)
    /// </summary>
    virtual void processEvents() override;

    /// <summary>
    /// Called once every game tick
    /// </summary>
    /// <param name="t_dTime">Time in seconds since the last update</param>
    virtual void update(sf::Time t_dT) override;

    void findPath();

    Point mouseClickToPoint(sf::Vector2f t_mousePos);

    void placePlayer(sf::Vector2f t_position);

    void placeNPC(sf::Vector2f t_position);

    void placeWall(sf::Vector2f t_position);

    /// <summary>
    /// Draw the game to the render window
    /// </summary>
    virtual void render() override;

private:

    const sf::Vector2f BOARD_OFFSET = { 460.f, 0.f };

    const int ROWS = 30;
    const int COLS = 30;

    const sf::Vector2i GRID_DIMENSIONS = { 1000, 1000 };
    const float CELL_WIDTH = GRID_DIMENSIONS.x / (float)COLS;
    const float CELL_HEIGHT = GRID_DIMENSIONS.y / (float)ROWS;

    Graph* m_graph;

    Pathfinder* m_pathfinder;

    sf::Sprite m_background;

    sf::Sprite m_player;
    std::vector<sf::Sprite> m_NPCs;
    std::vector<sf::Sprite> m_walls;
};
#endif