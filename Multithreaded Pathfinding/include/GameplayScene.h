#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "IBaseScene.h"
#include "Globals.h"
#include "Graph.h"
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

    void placePlayer(sf::Vector2f t_position);

    void placeNPC(sf::Vector2f t_position);

    /// <summary>
    /// Draw the game to the render window
    /// </summary>
    virtual void render() override;

private:
    Graph* m_graph;

    sf::Sprite m_background;

    sf::Sprite m_player;
    std::vector<sf::Sprite> m_NPCs;
};
#endif