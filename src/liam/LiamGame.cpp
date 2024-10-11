//
// Created by Liam Palmqvist on 07/10/2024.
//

// This is the main file for the "Game"

#include "LiamGame.h"

LiamGame::LiamGame() : running(true), paused(false), gameTime(0.0) {
    // the main instance of the "Game"
    std::shared_ptr<GL_XCube2dEngine> engine = GL_XCube2dEngine::getInstance();

    // engine ready, get subsystems
    gfx = engine->getGraphicsEngine();
    sfx = engine->getAudioInstance();
    eventSystem = engine->getEventInstance();
}

LiamGame::~LiamGame() {
    // kill Game class' instance pointers
    // so that engine is isolated from the outside world
    // before shutting down
    gfx.reset();
    eventSystem.reset();

    // kill engine
    GL_XCube2dEngine::quit();
}

int LiamGame::runMainLoop() {
#ifdef __DEBUG
    debug("Entered Main Loop in LiamGame");
#endif

    while (running) {
        gfx->setFrameStart();
        eventSystem->pollEvents();

        if (eventSystem->isPressed(Key::ESC) || eventSystem->isPressed(Key::QUIT))
            running = false;

        gfx->clearScreen();
        render();
        // renderUI();
        gfx->showScreen();

        gfx->adjustFPSDelay(16); // hard coded to ~60 FPS at the moment
    }

#ifdef __DEBUG
    debug("Exited Main Loop in LiamGame");
#endif

    return 0;
}

void LiamGame::render() {
    /*
    GLfloat vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };
    gfx->drawRect(vertices);
    */
    glEnableVertexAttribArray(gfx->attribute_coord2d);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(gfx->attribute_coord2d);
}

void LiamGame::update() {

}

void LiamGame::renderUI() {
    gfx->showScreen();
}
