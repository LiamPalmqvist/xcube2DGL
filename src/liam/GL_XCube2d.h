//
// Created by Liam Palmqvist on 06/10/2024.
//

#ifndef MYGAME_GL_XCUBE2D_H
#define MYGAME_GL_XCUBE2D_H

#include <memory>

#include "GL_GraphicsEngine.h"
#include "../engine/AudioEngine.h"
#include "../engine/EventEngine.h"
#include "../engine/PhysicsEngine.h"

class GL_XCube2dEngine {
private:
    // this is the shared instance of the entire engine
    static std::shared_ptr<GL_XCube2dEngine> instance;

    // This is my own graphics engine
    std::shared_ptr<GL_GraphicsEngine> gfxInstance;

    // since these aren't OpenGL, I can use them without worrying about modifying them
    // the shared instances of the Audio and Event system run using SDL but only as
    // audio and event backend, not rendering
    // I might not even need the audio engine since I don't know if I'll have any sound
    std::shared_ptr<AudioEngine> audioInstance;
    std::shared_ptr<EventEngine> eventInstance;

    GL_XCube2dEngine();
public:
    /**
     * @return the instance of game engine
     * @exception throws EngineException if init of any submodules failed
     */
    static std::shared_ptr<GL_XCube2dEngine> getInstance();

    /**
    * Quits the engine, closes all the subsystems
    *
    * All subsequent calls to any of subsystems will have undefined behaviour
    */
    static void quit();

    /**
     * Subsystems can only be accessed via the following accessors
     * @return appropriate subsystem of the engine
     */
    std::shared_ptr<GL_GraphicsEngine> getGraphicsEngine() { return gfxInstance; }
    std::shared_ptr<AudioEngine> getAudioInstance() { return audioInstance; }
    std::shared_ptr<EventEngine> getEventInstance() { return eventInstance; }
};


#endif //MYGAME_GL_XCUBE2D_H
