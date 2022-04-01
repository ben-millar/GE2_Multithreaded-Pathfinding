#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <chrono>

#include "IBaseScene.h"
#include "Globals.h"
#include "Graph.h"
#include "GraphRenderer.h"
#include "Pathfinder.h"
#include "SceneManager.h"
#include "thread_pool.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

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

    /// <summary>
    /// Wrap our member function call as an invokeable functor for passing into our threadpool
    /// </summary>
    /// <param name="t_instance"></param>
    /// <param name="t_func"></param>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <param name="g"></param>
    /// <returns></returns>
    auto wrap(Pathfinder* t_instance, Pathfinder::Path(Pathfinder::* t_func)(int, int, Graph const*), int a, int b, Graph const* g) ->
        std::function<Pathfinder::Path()>;

    void findPath();

    int mouseClickToIndex(sf::Vector2f t_mousePos);

    /// <summary>
    /// Draw the game to the render window
    /// </summary>
    virtual void render() override;

private:

    //template <typename F, typename... Args>
    //auto startTask(F&& f, Args&&... args)
    //{
    //    return m_threadPool->submit((std::forward<F>(f), std::forward<Args>(args)...));
    //}

    thread_pool* m_threadPool;

    Graph* m_graph;
    GraphRenderer* m_graphRenderer;

    Pathfinder* m_pathfinder;

    sf::FloatRect m_screenBounds{ 0.f, 0.f, 1080.f, 1080.f };

    int m_player{ 0 };

    std::vector<int> m_NPCs;
};
#endif