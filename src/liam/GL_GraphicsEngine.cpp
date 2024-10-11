//
// Created by Liam Palmqvist on 06/10/2024.
//

#include "GL_GraphicsEngine.h"

SDL_GLContext gl_context = nullptr;
// I'm going to hard code this temporarily
static const GLchar* vertexSource =
"#version 120\n"
"attribute vec2 coord2d;\n"
"void main() {\n"
"    gl_Position = vec4(coord2d, 0.0, 1.0);\n"
"}\n";

static const GLchar* fragmentSource =
"#version 120\n"
"void main() {\n"
"    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

GLuint program;
GLuint vbo;
GLfloat vertices[] = {
    0.0f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
};


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
            SDL_WINDOW_OPENGL
    );
    // By sending the flag `SDL_WINDOW_OPENGL` we specify that we want the window to
    // be rendered using OpenGL. This only works if we create a context for it
    // after

    // Checking if the window has been created or not
    if (nullptr == window) {
        throw EngineException("Failed to create window", SDL_GetError());
    }

    // Creating the context so that we can actually have stuff on the window
    gl_context = SDL_GL_CreateContext(window);

    if (nullptr == gl_context) {
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

    program = GL_GraphicsEngine::common_get_shader_program(vertexSource, fragmentSource);
    attribute_coord2d = glGetAttribLocation(program, "coord2d");
    std::cout << "Program: " << program << std::endl;

    /* Buffer setup. */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Global draw state */
    glUseProgram(program);
    //glViewport(0, 0, WIDTH, HEIGHT);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* Buffer setup. */
    
    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    printf("%u\n", vbo);
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

GLuint GL_GraphicsEngine::common_get_shader_program(
    /* Takes a vertex shader source and a fragment shader source
    *  and compiles them into a shader program ready to use
    */

    const char* vertex_shader_source,
    const char* fragment_shader_source
) {
    GLchar* log = NULL;
    GLint log_length, success;
    GLuint fragment_shader, program, vertex_shader;

    /* Vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

    /*
    log = malloc(log_length);
    if (log_length > 0) {
        glGetShaderInfoLog(vertex_shader, log_length, NULL, log);
        printf("vertex shader log:\n\n%s\n", log);
    }
    if (!success) {
        printf("vertex shader compile error\n");
        exit(EXIT_FAILURE);
    }
    */

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

    /*
    if (log_length > 0) {
        log = realloc(log, log_length);
        glGetShaderInfoLog(fragment_shader, log_length, NULL, log);
        printf("fragment shader log:\n\n%s\n", log);
    }
    if (!success) {
        printf("fragment shader compile error\n");
        exit(EXIT_FAILURE);
    }
    */

    /* Link shaders */
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glBindFragDataLocation(program, 0, "position");
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    /*
    if (log_length > 0) {
        log = realloc(log, log_length);
        glGetProgramInfoLog(program, log_length, NULL, log);
        printf("shader link log:\n\n%s\n", log);
    }
    if (!success) {
        printf("shader link error");
        exit(EXIT_FAILURE);
    }
    */

    std::cout << "Vertex Shader: " << vertex_shader << std::endl;
    std::cout << "Fragment Shader: " << fragment_shader << std::endl;
    std::cout << "Program: " << program << std::endl;

    /* Cleanup. */
    free(log);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if (vertices == NULL) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        std::cout << "vertices empty" << std::endl;
    }
    glVertexAttribPointer(glGetFragDataLocation(program, "position"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(program);

    return program;
}

void GL_GraphicsEngine::drawRect(GLfloat* vertices) {
    glEnableVertexAttribArray(glGetFragDataLocation(program, "position"));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(glGetFragDataLocation(program, "position"));
}

//
//void GL_GraphicsEngine::drawRect(GLfloat * vertices) {
//    // create an object containing Vertex Array Objects
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//
//    // Bind it to start using it
//    //glBindVertexArray(vao);
//
//    // NOW we can actually generate an object to display on screen using the parsed vertices
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    
//
//    // be aware this is NOT a fragment shader. This works on shading VERTEXES AKA 3D MATH OBJECTS
//    // NOT pixels
//    // Creating the vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    // giving the shader source, number of shaders, and the length
//    glShaderSource(vertexShader, 1, &vertexSource, NULL);
//
//    glCompileShader(vertexShader);
//
//    // Since GLSL shader code does not give syntax errors, we need to manually output them ourselves
//    GLint vertexStatus;
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);
//
//    // What follows IS a fragment shader. These can be used in tandem with vertexShaders by compiling them
//    // into a shader program
//    
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &vertexSource, NULL);
//
//    // more error stuff
//    GLint fragmentStatus;
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentStatus);
//
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);
//
//
//    // In order to actually USE these shaders in the program we need to create what's called
//    // a Shader Program
//    // This LINKS the shaders to a computed program which, when run by the renderer, computes an output
//    /*
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glBindFragDataLocation(shaderProgram, 0, "outColor");
//    glLinkProgram(shaderProgram);
//    glUseProgram(shaderProgram);
//    */
//
//    // This gets the "position" values inside the shader I created
//    GLint posAttrib = glGetAttribLocation(program, "position");
//
//    // enable the vertex attribute array
//    glEnableVertexAttribArray(posAttrib);
//    std::cout << posAttrib << std::endl;
//
//    // and this assigns them to the data we passed to the function
//    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//}
