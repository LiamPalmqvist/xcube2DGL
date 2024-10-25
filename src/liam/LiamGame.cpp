//
// Created by Liam Palmqvist on 07/10/2024.
//

// This is the main file for the "Game"

#include "LiamGame.h"
GLfloat dynamicVertices[] = {
    -0.01f, 0.01f,
    0.01f, -0.01f,
    -0.01f, -0.01f,
    0.01f, 0.01f
};

GLfloat screenBorders[]{
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f
};

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
    //gfx->reloadShaders();
#endif

    while (running) {
        
        gfx->setFrameStart();
        eventSystem->pollEvents();

        if (eventSystem->isPressed(Key::ESC) || eventSystem->isPressed(Key::QUIT))
            running = false;
        
        if (eventSystem->isPressed(Key::SPACE)) gfx->reloadShaders();

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
    // This draws a triangle at the screen borders at
    // [Top right]
    // [Bottom left]
    // [Bottom right]
    //screenBorders[0] -= 0.1;
    //screenBorders[3] += 0.1;
    //screenBorders[5] -= 0.1;
    //if (screenBorders[0] < -1.1) screenBorders[0] = 1;
    //if (screenBorders[3] > 1.1) screenBorders[3] = -1;
    //if (screenBorders[5] < -1.1) screenBorders[5] = 1;
    GLfloat colour[4] = { 1.0f, 0.0f, 0.0f, 0.1f };
    gfx->updateTime();
    //gfx->drawRect(screenBorders, colour);
    gfx->liam_drawRect();
    //gfx->drawRect(dynamicVertices, colour);
    
    //glEnableVertexAttribArray(gfx->attribute_coord2d);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDisableVertexAttribArray(gfx->attribute_coord2d);
}

void LiamGame::update() {

}

void LiamGame::renderUI() {
    gfx->showScreen();
}
