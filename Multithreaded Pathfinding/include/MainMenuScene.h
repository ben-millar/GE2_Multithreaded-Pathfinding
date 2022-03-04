#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "IBaseScene.h"
#include "Globals.h"

class MainMenuScene :
    public IBaseScene
{
public:

    MainMenuScene();
    ~MainMenuScene();

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
    /// Draw the game to the render window
    /// </summary>
    virtual void render() override;

private:

};
#endif