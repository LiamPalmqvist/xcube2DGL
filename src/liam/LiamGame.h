//
// Created by Liam Palmqvist on 07/10/2024.
//

#ifndef MYGAME_LIAMGAME_H
#define MYGAME_LIAMGAME_H

#include "../engine/XCube2d.h"

// This is what's gonna be in the main file (I think)
class LiamGame {
protected:

	std::shared_ptr<XCube2Engine> engine = XCube2Engine::getInstance();

    /* Engine systems */
    std::shared_ptr<GL_GraphicsEngine> gfx;
    std::shared_ptr<AudioEngine> sfx;
    std::shared_ptr<EventEngine> eventSystem;
    std::shared_ptr<PhysicsEngine> physics;

    /* Main loop control */
    bool running;
    bool paused;
    double gameTime;
    

    void update();
    void render();

    // Since nothing is written in "../engine/AbstractGame.cpp"
    // I need to define it myself
    void renderUI();

public:
    int runMainLoop();

    LiamGame();

    ~LiamGame();
};


#endif //MYGAME_LIAMGAME_H
