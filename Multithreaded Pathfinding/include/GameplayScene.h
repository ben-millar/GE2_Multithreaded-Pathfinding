#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <chrono>

#include "IBaseScene.h"
#include "Globals.h"
#include "Graph.h"
#include "GraphRenderer.h"
#include "Pathfinder.h"
#include "SceneManager.h"
#include "ThreadPool.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace std::chrono_literals;

class GameplayScene :
    public IBaseScene
{
public:
    GameplayScene(int t_gridSize, int t_numNPCs);
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

    void drawPath(std::shared_ptr<Pathfinder::Path> t_path);

    int mouseClickToIndex(sf::Vector2f t_mousePos);

    /// <summary>
    /// Draw the game to the render window
    /// </summary>
    virtual void render() override;

private:
    void scrollView(sf::Time t_dT);

    const int GRID_SIZE;
    const int NUM_NPCS;

    ThreadPool* m_threadPool;

    Graph* m_graph;
    GraphRenderer* m_graphRenderer;

    Pathfinder** m_pathfinders;

    sf::FloatRect m_screenBounds{ 0.f, 0.f, 1080.f, 1080.f };

    int m_player{ 0 };

    std::vector<int> m_NPCs;

    bool m_placingWalls{false}, m_removingWalls{false};
};
#endif