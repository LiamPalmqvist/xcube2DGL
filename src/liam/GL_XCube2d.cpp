//
// Created by Liam Palmqvist on 06/10/2024.
//

#include "GL_XCube2d.h"

std::shared_ptr<GL_XCube2dEngine> GL_XCube2dEngine::instance = nullptr;

GL_XCube2dEngine::GL_XCube2dEngine() {
    std::cout << "Initializing X-CUBE 2D OpenGL" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    // Debug information
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw EngineException("SDL_Init()", SDL_GetError());

    SDL_version compiled, linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    printf("Compiled against SDL %d.%d.%d\n",
           compiled.major, compiled.minor, compiled.patch);
    printf("Linked against SDL %d.%d.%d\n",
           linked.major, linked.minor, linked.patch);
    // no more debug information


    Uint32 ticks = SDL_GetTicks();
    srand(ticks); // Random seed

    // init subsystems
    // in GL_GraphicsEngine, friend class GL_XCube2dEngine is required
    gfxInstance = std::shared_ptr<GL_GraphicsEngine>(new GL_GraphicsEngine());

    // Same with these
    audioInstance = std::shared_ptr<AudioEngine>(new AudioEngine());

    eventInstance = std::shared_ptr<EventEngine>(new EventEngine());
}

void GL_XCube2dEngine::quit() {
    if (instance)
        instance.reset();
}

std::shared_ptr<GL_XCube2dEngine> GL_XCube2dEngine::getInstance() {
    if (!instance)
        instance = std::shared_ptr<GL_XCube2dEngine>(new GL_XCube2dEngine());
    return instance;
}