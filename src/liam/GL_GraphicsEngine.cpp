//
// Created by Liam Palmqvist on 06/10/2024.
//

#include "GL_GraphicsEngine.h"

SDL_GLContext glContext = nullptr;

/**
 * This is the class initialiser, it creates the SDL window and the context
 * which we will use to draw and render the screen using OpenGL's drawing
 * frameworks
 * We also set some default values as we call the initialiser
 */
GL_GraphicsEngine::GL_GraphicsEngine() : fpsAverage(0), fpsPrevious(0), fpsStart(0), fpsEnd(0) {
    // set up GlewExperimental context so that I can actually render OpenGL Contexts using GLEW
    glewExperimental = GL_TRUE;

    window = SDL_CreateWindow(
            "The X-CUBE 2D Game Engine - OpenGL Version",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    // By sending the flag `SDL_WINDOW_OPENGL` we specify that we want the window to
    // be rendered using OpenGL. This only works if we create a context for it
    // after

    // Checking if the window has been created or not
    if (nullptr == window) {
        throw EngineException("Failed to create window", SDL_GetError());
    }

    // Creating the context so that we can actually have stuff on the window
    glContext = SDL_GL_CreateContext(window);

    if (nullptr == glContext) {
        throw EngineException("Failed to create OpenGL context", SDL_GetError());
    }

    // although not necessary, SDL doc says to prevent hiccups load it before using
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        throw EngineException("Failed to init SDL_image - PNG", IMG_GetError());
    }

    if (TTF_Init() < 0) {
        throw EngineException("Failed to init SDL_ttf", TTF_GetError());
    }

    // actually create the GLEW context
    glewInit();

    // set up GLEW int vertex buffers
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    printf("%u\n", vertexBuffer);
}

// literally destroys and quits everything. I don't know if SGL_GL_DeleteContext is req
GL_GraphicsEngine::~GL_GraphicsEngine() {
    IMG_Quit();
    TTF_Quit();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GL_GraphicsEngine::setFrameStart() {
    fpsStart = SDL_GetTicks();
}

void GL_GraphicsEngine::clearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GL_GraphicsEngine::showScreen() {
    // Swapping the active buffers
    SDL_GL_SwapWindow(window);
}

// Setting FPS?
void GL_GraphicsEngine::adjustFPSDelay(const Uint32 &delay) {
    fpsEnd = SDL_GetTicks() - fpsStart;
    if (fpsEnd < delay) {
        SDL_Delay(delay - fpsEnd);
    }
}

void testDraw() {

}

void GL_GraphicsEngine::drawRect(GLfloat * vertices) {
    // create an object containing Vertex Array Objects
    //GLuint vao;
    //glGenVertexArrays(1, &vao);

    // Bind it to start using it
    //glBindVertexArray(vao);
    
    GLuint vbo;
    // first, make an empty buffer
    glGenBuffers(1, &vbo);
    // then, generate the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // finally, make it the active buffer and bind it as a GL_ARRAY_BUFFER

    // NOW we can actually generate an object to display on screen using the parsed vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // After generating the data used to draw things to the screen, we need to write a shader to make these
    // vertices visible
    // I'm going to hard code this temporarily
    const char* vertexSource = R"glsl(
        #version 150 core
        in vec2 position;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )glsl";

    // be aware this is NOT a fragment shader. This works on shading VERTEXES AKA 3D MATH OBJECTS
    // NOT pixels
    // Creating the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // giving the shader source, number of shaders, and the length
    glShaderSource(vertexShader, 1, &vertexSource, NULL);

    glCompileShader(vertexShader);

    // Since GLSL shader code does not give syntax errors, we need to manually output them ourselves
    GLint vertexStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);

    // What follows IS a fragment shader. These can be used in tandem with vertexShaders by compiling them
    // into a shader program
    const char* fragmentSource = R"glsl(
        #version 150 core
        out vec4 outColor;

        void main() {
            outColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )glsl";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &vertexSource, NULL);

    // more error stuff
    GLint fragmentStatus;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentStatus);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);


    // In order to actually USE these shaders in the program we need to create what's called
    // a Shader Program
    // This LINKS the shaders to a computed program which, when run by the renderer, computes an output
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // This gets the "position" values inside the shader I created
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");

    // enable the vertex attribute array
    glEnableVertexAttribArray(posAttrib);

    // and this assigns them to the data we passed to the function
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    

    std::cout << "drawing rect" << std::endl;
}