#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "IBaseScene.h"
#include "Globals.h"
#include "Graph.h"
#include "GraphRenderer.h"
#include "Pathfinder.h"
#include "SceneManager.h"

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

    int mouseClickToIndex(sf::Vector2f t_mousePos);

    /// <summary>
    /// Draw the game to the render window
    /// </summary>
    virtual void render() override;

private:

    Graph* m_graph;
    GraphRenderer* m_graphRenderer;

    Pathfinder* m_pathfinder;

    sf::FloatRect m_screenBounds{ 0.f, 0.f, 1080.f, 1080.f };

    int m_player;
    int m_npc;
};
#endif