//
// Created by Liam Palmqvist on 06/10/2024.
//

#ifndef MYGAME_GL_GRAPHICSENGINE_H
#define MYGAME_GL_GRAPHICSENGINE_H
//#define GLEW_STATIC
// Since we're including GLEW as a static library already, we don't need to define it as static where we're using it
// otherwise we'd uncomment this line

#include <string>
#include <memory>
#include <iostream>
#include <chrono>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <gl/glew.h>
#include <GL/GL.h>

#include "FileReader.h"
#include "../engine/EngineCommon.h"
#include "../engine/GameMath.h"

/* ENGINE DEFAULT SETTINGS */
// since these are already defined in another file, I don't need them here
// but I'll only comment them out because it shows what I know


static const int DEFAULT_WINDOW_WIDTH = 800;
static const int DEFAULT_WINDOW_HEIGHT = 600;
/*
static const SDL_Color SDL_COLOR_GRAY	= { 0x80, 0x80, 0x80 };
static const SDL_Color SDL_COLOR_YELLOW = { 0xFF, 0xFF, 0x00 };
static const SDL_Color SDL_COLOR_RED	= { 0xFF, 0x00, 0x00 };
static const SDL_Color SDL_COLOR_GREEN	= { 0x00, 0xFF, 0x00 };
static const SDL_Color SDL_COLOR_BLUE	= { 0x00, 0x00, 0xFF };
static const SDL_Color SDL_COLOR_BLACK  = { 0x00, 0x00, 0x00 };
static const SDL_Color SDL_COLOR_WHITE  = { 0xFF, 0xFF, 0xFF };
static const SDL_Color SDL_COLOR_AQUA   = { 0x00, 0xFF, 0xFF };
static const SDL_Color SDL_COLOR_ORANGE = { 0xFF, 0xA5, 0x00 };
static const SDL_Color SDL_COLOR_PINK   = { 0xFF, 0xC0, 0xCB };
static const SDL_Color SDL_COLOR_PURPLE = { 0x80, 0x00, 0x80 };
static const SDL_Color SDL_COLOR_VIOLET = { 0xEE, 0x82, 0xEE };

inline SDL_Color getRandomColor(int minRGB, int maxRGB) {
    SDL_Color color = { (Uint8)getRandom(minRGB, maxRGB), (Uint8)getRandom(minRGB, maxRGB), (Uint8)getRandom(minRGB, maxRGB) };
    return color;
}

struct SDL_Colorf {
    float r, g, b, a;
};

inline SDL_Colorf toSDLColorf(SDL_Color color) {
    SDL_Colorf colorf = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
    return colorf;
}

inline SDL_Color toSDLColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Color color = { r, g, b, a };
    return color;
}
*/


using namespace std;

// This class will be defined not using SDL's built-in render pipeline but instead
// an OpenGL context to which I will be able to write to directly
class GL_GraphicsEngine {
	friend class XCube2dEngine;
private:
    SDL_Window * window;
    SDL_GLContext gl_Context = nullptr;

    // This is the colour we will draw to the screen
    SDL_Color drawColor;

    // the font we will draw to the screen. It is hard coded at the moment
    TTF_Font * font = nullptr;

    // definitions for fps trackers
    Uint32 fpsAverage, fpsPrevious, fpsStart, fpsEnd;
    SDL_Surface * image = nullptr;

	// definitions for buffer objects
	GLuint vao;             // Vertex   Array   Object
	GLuint vbo;             // Vertex   Buffer  Object
	GLuint ebo;             // Element  Buffer  Object
	GLuint tex;             // Texture  Buffer  Object
	GLuint program;         // Shader   Program Object

    // the frame buffers for vertices, colours and textures
	GLint posAttrib;        // Position   Attribute
	GLint colAttrib;        // Colour     Attribute
	GLint texAttrib;        // Texture    Attribute
	GLint deltaTimeAttrib;  // Delta Time Attribute
	GLint timeAttrib;       // Time       Attribute
    GLint resAttrib;        // Resolution Attribute
    GLint progAttrib;       // Progress   Attribute

    FileReader reader;      // Reader   Object
    string textureFragment; // Texture  string
    int windowHeight;       // Window   Height
    int windowWidth;        // Window   Width


public:
    GL_GraphicsEngine();
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point currentTime;
    float deltaTime;
    float progress;
    bool reverse = false;
    GLint attribute_coord2d;
    GLint attribute_colour;
    GLfloat attribute_time;
    const GLchar* shaderOutput;
    string output = "";
    
    ~GL_GraphicsEngine();

    //void useFont(TTF_Font * font);

    /**
     * Clears everything on the screen
     * Call this before drawing anything to the screen
     */
     //void clearScreen();

     /**
      * Displays everything rendered on the screen
      * Call this method after drawing has been finished
      */
      //void showDrawnScreen();
    void setFrameStart();

    static void clearScreen();

    void showScreen();

    void adjustFPSDelay(const Uint32 &delay);
    
    void setVerticalSync(bool);

    void useFont(TTF_Font * _font);

    void get_shader_program(
        const char* vertex_shader_source,
        const char* fragment_shader_source,
		const GLfloat texture_image_source[] = nullptr
    );

    void reloadShaders();

    void updateTime();

    void drawTri(GLfloat verts[], GLfloat color[]);
    void drawRect(GLfloat verts[], GLfloat colour[]);
    void drawShader();
};


#endif //MYGAME_GL_GRAPHICSENGINE_H
