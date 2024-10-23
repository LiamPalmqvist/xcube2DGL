//
// Created by Liam Palmqvist on 06/10/2024.
//

#include "GL_GraphicsEngine.h"

SDL_GLContext gl_context = nullptr;
static const GLchar* vertexSource = R"glsl(
    #version 410

    in vec2 coord2d;

    void main() {
        gl_Position = vec4(coord2d, 0.0, 1.0);
    }
)glsl";

static const GLchar* fragmentSource = R"glsl(
    #version 410
    precision mediump float;

    uniform vec2 u_resolution = vec2(800, 600);
    uniform float u_time = 0;

    void main() {
        vec2 st = gl_FragCoord.xy / u_resolution.xy;
        st.x *= u_resolution.x / u_resolution.y;
        vec3 color = vec3(0.0);
        color = vec3(0, st.x, /*(st.x+st.y)*/ abs(sin(u_time)));
        gl_FragColor = vec4(color, 1.0);
    }
)glsl";

static const GLchar* newVertices = R"glsl(
    #version 410 core

    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    
    out vec3 Color;
    out vec2 Texcoord;

    void main() {
        Color = color;
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

static const GLchar* newFrag = R"glsl(
    #version 410 core

    in vec3 Color;
    in vec2 Texcoord;
    uniform sampler2D tex;

    void main() {
        gl_FragColor = texture(tex, Texcoord) * vec4(Color, 1.0);
    }
)glsl";



// black and white checkerboard
static const GLfloat pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
};

GLuint program;
GLuint vbo; // This is the vertex buffer object
GLuint ebo; // This is the entity buffer object
GLuint tex; // This is the texture buffer object
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
    // Start tracking time
    startTime, currentTime = chrono::high_resolution_clock::now();

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

    cout << "GL Version: " << glGetString(GL_VERSION) << endl;

    /* Buffer setup. */
    glGenBuffers(1, &vbo); // Generate the buffer to use with the vertices
    glGenBuffers(1, &ebo); // Generate the buffer to use with the entities
    glGenTextures(1, &tex); // Generate the buffer to use with the loaded textures
    
    
    // grabbing context from compiled shaders
    GL_GraphicsEngine::liam_get_shader_program(newVertices, newFrag);
    //GL_GraphicsEngine::common_get_shader_program(vertexSource, fragmentSource, pixels);
    

    //GLint uniColor = glGetUniformLocation(program, "triangleColor");
    //glUniform3f(uniColor, 1.0f, 1.0f, 0.0);
    std::cout << "Program: " << program << std::endl;

    // this could be moved into drawRect()
    

    /* Global draw state */
    //glUseProgram(program);
    //glViewport(0, 0, WIDTH, HEIGHT);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    
    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // =====================================================================
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

void GL_GraphicsEngine::liam_get_shader_program(
    const char* vertex_shader_source,
    const char* fragment_shader_source
) {
    // create vars
    GLchar* log = NULL;
    GLint log_length, success;
    GLuint program, vertex_shader, fragment_shader;

    // compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

    log = (GLchar*)malloc(log_length);
    if (log_length > 0) {
        glGetShaderInfoLog(vertex_shader, log_length, NULL, log);
        printf("vertex shader log:\n\n%s\n", log);
    }
    if (!success) {
        printf("vertex shader compile error\n");
        exit(EXIT_FAILURE);
    }

    // compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0) {
        log = (GLchar*)realloc(log, log_length);
        glGetShaderInfoLog(fragment_shader, log_length, NULL, log);
        printf("fragment shader log:\n\n%s\n", log);
    }
    if (!success) {
        printf("fragment shader compile error\n");
        exit(EXIT_FAILURE);
    }

    // load image
    int width, height;
    SDL_Surface* image = IMG_Load("sample.png");


    // create a program and link everything to it
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);
    
    posAttrib = glGetUniformLocation(program, "position");
    colAttrib = glGetUniformLocation(program, "color");
    texAttrib = glGetUniformLocation(program, "texcoord");
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colAttrib);
    glEnableVertexAttribArray(texAttrib);

    // link image
    glBindTexture(GL_TEXTURE_2D, tex);
    // Type, 0, colour space, width, height, 0, colour depth?, type of information, information
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    // image params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (posAttrib == -1) {
        cout << "Position attribute not found" << endl;
    }
    else {
        cout << "Position attribute found" << endl;
    }
    if (colAttrib == -1) {
        cout << "Colour attribute not found" << endl;
    }
    else {
        cout << "Colour attribute found" << endl;
    } 
    if (texAttrib == -1) {
        cout << "Texture attribute not found" << endl;
    }
    else {
        cout << "Texture attribute found" << endl;
    }

    glUseProgram(program);
}

GLuint GL_GraphicsEngine::common_get_shader_program(
    /* Takes a vertex shader source and a fragment shader source
    *  and compiles them into a shader program ready to use
    */

    const char* vertex_shader_source,
    const char* fragment_shader_source,
    const GLfloat texture_image_source[]
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

    
    log = (GLchar *)malloc(log_length);
    if (log_length > 0) {
        glGetShaderInfoLog(vertex_shader, log_length, NULL, log);
        printf("vertex shader log:\n\n%s\n", log);
    }
    if (!success) {
        printf("vertex shader compile error\n");
        exit(EXIT_FAILURE);
    }
    

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

    
    if (log_length > 0) {
        log = (GLchar *)realloc(log, log_length);
        glGetShaderInfoLog(fragment_shader, log_length, NULL, log);
        printf("fragment shader log:\n\n%s\n", log);
    }
    if (!success) {
        printf("fragment shader compile error\n");
        exit(EXIT_FAILURE);
    }
    

    /* Link shaders */
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    //glBindFragDataLocation(program, 0, "position");
    //glBindFragDataLocation(program, 0, "triangleColor");
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

    attribute_coord2d = glGetAttribLocation(program, "coord2d");
    attribute_colour = glGetUniformLocation(program, "triangleColor");
    attribute_time = glGetUniformLocation(program, "u_time");

    std::cout << "Vertex Shader: " << vertex_shader << std::endl;
    std::cout << "Fragment Shader: " << fragment_shader << std::endl;
    std::cout << "Program: " << program << std::endl;

    /* link image */
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, texture_image_source);


    /* Cleanup. */
    free(log);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    //glGenBuffers(1, &vbo); // we don't need to regenerate the buffer object
    // bind the buffers to be worked in the next line
    //glBindBuffer(GL_ARRAY_BUFFER, vbo); // nor do we need to bind it again
    // if the vertices is NOT NULL
    if (vertices != NULL) {
        // send the vertices to the buffer to be drawn statically
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        std::cout << "vertices not empty" << std::endl;
    }
    // get the attribute pointer for the "position" of the vertices
    glVertexAttribPointer(glGetFragDataLocation(program, "position"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    // and assign the array of vertices to the active buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(program);

    return program;
}

void GL_GraphicsEngine::reloadShaders() {
    string fileName = "shader.fragment";
    FileReader reader;
    const GLchar* shaderOutput;
    
    shaderOutput = reader.ReadFile(fileName)->c_str();
    //std::cout << shaderOutput << endl;
    
    //attribute_colour = glGetUniformLocation(program, "triangleColor");
    // since the colours are generated inside of the shader
    // we don't need to use this.

    // load the texture we made here
    string texName = "texture.png";
    const GLfloat* textureOutput;
    textureOutput = pixels;
    // since the texture and function don't currently exist
    // this is commented out
    // textureOutPut = reader.ReadTexture(texName);
    
    // load the shaders we grabbed
    // This might have an image param later

    common_get_shader_program(vertexSource, shaderOutput, textureOutput);
    attribute_coord2d = glGetAttribLocation(program, "coord2d");
    attribute_time = glGetUniformLocation(program, "u_time");

    int count;
    int size;
    GLenum type;
    const GLsizei bufSize = 16;
    GLsizei length;
    GLchar name[bufSize];
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    for (int i = 0; i < count; i++) {
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
        cout << "Uniform: " << i << " Type: " << type << " Name: " << name << endl;
    }
}

void GL_GraphicsEngine::updateTime() {
    currentTime = chrono::high_resolution_clock::now();
    deltaTime = chrono::duration_cast<chrono::duration<float>>(currentTime - startTime).count();
}

void GL_GraphicsEngine::drawTri(GLfloat verts[], GLfloat colour[]) {
    
    GLfloat r = colour[0];
    GLfloat g = colour[1];
    GLfloat b = colour[2];
    GLfloat a = 1.0f;

    std::cout << sizeof(colour);

    // check if there is alpha
    if (sizeof(colour) == 8) {
        a = colour[3];
    }

    /* Buffer setup. */
    for (int i = 0; i < sizeof(verts) / GLfloat(1); i++) {
        vertices[i] = verts[i];
    }
    
    // bind the vertex buffer object to the GL_ARRAY_BUFFER
    // which is what OpenGL uses to generate points on a screen
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // and set our colour uniform to the parsed colours
    //glUniform4f(attribute_colour, r, g, b, a);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // first, we need to assign the new verts to the globally drawn vertices
    glEnableVertexAttribArray(attribute_coord2d);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);
}

void GL_GraphicsEngine::drawRect(GLfloat verts[8], GLfloat colour[]) {
    GLfloat r = colour[0];
    GLfloat g = colour[1];
    GLfloat b = colour[2];
    GLfloat a = 1.0f;

    // because we know that we are going to be drawing a single rect
    // we can hard code the order the vertices are drawn
    GLuint elements[6] = {
        0, 1, 2, 
        2, 3, 0
    };

    // need an array the size of the one parsed
    // which will *always* be 8 because it's a single
    // rect taking 4 coords of 2 floats
    GLfloat rectVerts[8];

    // std::cout << sizeof(colour);

    // check if there is alpha
    if (sizeof(colour) == 8) {
        a = colour[3];
    }

    /* Buffer setup. */
    for (int i = 0; i < sizeof(verts) / GLfloat(1); i++) {
        rectVerts[i] = verts[i];
    }

    // generate the buffers needed (this could be done outside of the loop)
    // bind the buffer to the GL_ARRAY_BUFFER
    // this is what glDrawElements pulls from when drawing triangles
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // bind the vertices passed to the function to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVerts), rectVerts, GL_STATIC_DRAW);

    // bind the element buffer object to the GL_ELEMENT_ARRAY_OBJECT
    // which is what is used to reference the order to draw vertices
    // in the GL_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // bind the data in our elements array to the element buffer object
    // used by the GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //GLint uniColor = glGetUniformLocation(program, "triangleColor");
    glUniform4f(attribute_colour, r, g, b, a);
    //cout << deltaTime << endl;
    //cout << "Attribute_Time" << attribute_time << endl;
    glUniform1f(attribute_time, deltaTime);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
    // we also need to bind the textures whenever we reload the shader
    glBindTexture(GL_TEXTURE_2D, tex);
    // and make sure that the texture isn't stretched at all when imported
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // additionally, we must decide which filtering option to use when scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // while we CAN use mipmaps, the texture needs to be loaded before generating
    // so if we don't have an texture loaded, the program will crash.
    */
    
    // enable the vertices to be drawn
    glEnableVertexAttribArray(attribute_coord2d);
    // draw the elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // disable the vertices from being drawn
    glDisableVertexAttribArray(attribute_coord2d);
}

void GL_GraphicsEngine::liam_drawRect(GLfloat verts[8], GLfloat colour[]) {
    GLfloat r = colour[0];
    GLfloat g = colour[1];
    GLfloat b = colour[2];
    GLfloat a = 1.0f;

    // because we know that we are going to be drawing a single rect
    // we can hard code the order the vertices are drawn
    GLuint elements[6] = {
        0, 1, 2,
        2, 3, 0
    };

    // need an array the size of the one parsed
    // which will *always* be 8 because it's a single
    // rect taking 4 coords of 2 floats
    GLfloat rectVerts[8];

    // std::cout << sizeof(colour);

    // check if there is alpha
    if (sizeof(colour) == 8) {
        a = colour[3];
    }

    /* Buffer setup. */
    for (int i = 0; i < sizeof(verts) / GLfloat(1); i++) {
        rectVerts[i] = verts[i];
    }

    // generate the buffers needed (this could be done outside of the loop)
    // bind the buffer to the GL_ARRAY_BUFFER
    // this is what glDrawElements pulls from when drawing triangles
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // bind the vertices passed to the function to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVerts), rectVerts, GL_STATIC_DRAW);

    // bind the element buffer object to the GL_ELEMENT_ARRAY_OBJECT
    // which is what is used to reference the order to draw vertices
    // in the GL_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // bind the data in our elements array to the element buffer object
    // used by the GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //GLint uniColor = glGetUniformLocation(program, "triangleColor");
    glUniform4f(colAttrib, r, g, b, a);
    //cout << deltaTime << endl;
    //cout << "Attribute_Time" << attribute_time << endl;
    //glUniform1f(attribute_time, deltaTime);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // enable the vertices to be drawn
    glEnableVertexAttribArray(posAttrib);
    // draw the elements
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // disable the vertices from being drawn
    glDisableVertexAttribArray(posAttrib);
}

