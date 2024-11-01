//
// Created by Liam Palmqvist on 06/10/2024.
//

#include "GL_GraphicsEngine.h"

static const GLchar* textureVertex = R"glsl(
    #version 410
    
    // Input vertex data, different for all executions of this shader.
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
        
    // Output data - will be interpolated for each fragment.
    out vec3 Color;
    out vec2 Texcoord;

    void main() {
        Color = color;
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

static const GLchar* textureFragment = R"glsl(
uniform vec2 iResolution = vec2(800, 600);
uniform float iDeltaTime;
uniform float progress;
uniform float PI = 3.1415926;

out vec4 outColor;

vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}

//https://www.shadertoy.com/view/mtyGWy
void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + iDeltaTime*.4);

        d = sin(d*8. + iDeltaTime)/8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    outColor = vec4(finalColor, 1.0);
}
)glsl";


GLfloat vertices[] = {
	//   Position     Colour        Texcoords
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top - left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top - right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom - right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom - left
};

GLfloat bigVertices[] = {
//   Position     Colour            Texcoords
    -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top - left
     1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top - right
     1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom - right
    -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom - left
};

GLint elements[] = {
    0, 1, 2,
    2, 3, 0
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
            //1920, 1080,
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
    gl_Context = SDL_GL_CreateContext(window);

    if (nullptr == gl_Context) {
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
	glGenVertexArrays(1, &vao); // Generate the vertex array object
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo); // Generate the buffer to use with the vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glGenBuffers(1, &ebo); // Generate the buffer to use with the entities
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glGenTextures(1, &tex); // Generate the buffer to use with the loaded textures
    
    
    // grabbing context from compiled shaders
    GL_GraphicsEngine::liam_get_shader_program(textureVertex, textureFragment);
    //GL_GraphicsEngine::liam_get_shader_program(vertexSource, fragmentSource);
    //GL_GraphicsEngine::common_get_shader_program(vertexSource, fragmentSource, pixels);
	//GL_GraphicsEngine::common_get_shader_program(textureVertex, textureFragment, pixels);
    

    //GLint uniColor = glGetUniformLocation(program, "triangleColor");
    //glUniform3f(uniColor, 1.0f, 1.0f, 0.0);
    std::cout << "Program: " << program << std::endl;    

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
    SDL_GL_DeleteContext(gl_Context);
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
    const char* fragment_shader_source,
	const GLfloat texture_image_source[]
) {
    if (sizeof(fragment_shader_source) == NULL) {
        printf("Fragment shader source is NULL\n");
        return;
	}
	else if (sizeof(vertex_shader_source) == NULL) {
		printf("Vertex shader source is NULL\n");
		return;
	}

    // create vars
    GLchar* log = NULL;
    GLint log_length, success;
    GLuint vertex_shader, fragment_shader;

    // compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    
	// check for errors
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

    log = (GLchar*)malloc(log_length);
    if (log_length > 0) {
        glGetShaderInfoLog(vertex_shader, log_length, NULL, log);
        printf("vertex shader log:\n\n%s\n", log);
		printf("parsed vertex shader:\n\n%s\n", vertex_shader_source);
		printf("parsed vertex shader length: %d\n", sizeof(vertex_shader_source));
        free(log);
    }
    if (!success) {
        printf("vertex shader compile error\n");
        free(log);
        exit(EXIT_FAILURE);
    }


    // compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

	// check for errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0) {
        log = (GLchar*)realloc(log, log_length);
        glGetShaderInfoLog(fragment_shader, log_length, NULL, log);
        printf("fragment shader log:\n\n%s\n", log);
		//printf("parsed fragment shader:\n\n%s\n", fragment_shader_source);
        free(log);
        //cout << "parsed fragment shader length:" << sizeof(fragment_shader_source) << endl;
    }
    if (!success) {
        printf("fragment shader compile error\n");
        //free(log);
        return;
        //exit(EXIT_FAILURE);
    }

    // create a program and link everything to it
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);
    
	// specify the layout of the vertex data
    posAttrib = glGetAttribLocation(program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(posAttrib);
    
    colAttrib = glGetAttribLocation(program, "color");
    glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colAttrib);

    texAttrib = glGetAttribLocation(program, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texAttrib);

    timeAttrib = glGetUniformLocation(program, "iDeltaTime");

	progAttrib = glGetUniformLocation(program, "progress");

    resAttrib = glGetUniformLocation(program, "iResolution");

    // load image
    int width, height;
    SDL_Surface* image = IMG_Load("sample.png");
	if (nullptr == image) {
		printf("Failed to load image\n");
		exit(EXIT_FAILURE);
    }
    else {
		printf("Image loaded\n");
    }

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// link image since the texture buffer has been generated
    glBindTexture(GL_TEXTURE_2D, tex);
    //           target,        level, internal format,  width,    height,   border, format,  type,             data
    glTexImage2D(GL_TEXTURE_2D, 0,     GL_RGBA,          image->w, image->h, 0,      GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    // image params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// cleanup
    SDL_free(image);

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
    glUniform2i(resAttrib, 800, 600);
}


void GL_GraphicsEngine::common_get_shader_program(
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

    // load image
    int width, height;
    SDL_Surface* image = IMG_Load("sample.png");
    if (nullptr == image) {
        printf("Failed to load image\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Image loaded\n");
    }
    

    /* Link shaders */
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

    posAttrib = glGetAttribLocation(program, "position");
    colAttrib = glGetUniformLocation(program, "color");
    texAttrib = glGetUniformLocation(program, "texcoord");
	timeAttrib = glGetUniformLocation(program, "deltaTime");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colAttrib);
    glEnableVertexAttribArray(texAttrib);

    std::cout << "Vertex Shader: " << vertex_shader << std::endl;
    std::cout << "Fragment Shader: " << fragment_shader << std::endl;
    std::cout << "Program: " << program << std::endl;

    /* link image */
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Cleanup. */
    free(log);
    //glDeleteShader(vertex_shader);
    //glDeleteShader(fragment_shader);

    //glGenBuffers(1, &vbo); // we don't need to regenerate the buffer object
    // bind the buffers to be worked in the next line
    //glBindBuffer(GL_ARRAY_BUFFER, vbo); // nor do we need to bind it again
    // if the vertices is NOT NULL
    //if (vertices != NULL) {
    //    // send the vertices to the buffer to be drawn statically
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //    std::cout << "vertices not empty" << std::endl;
    //}
    // get the attribute pointer for the "position" of the vertices
    glVertexAttribPointer(glGetFragDataLocation(program, "position"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    // and assign the array of vertices to the active buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(program);
}

void GL_GraphicsEngine::reloadShaders() {
    string fileName = "shader.fragment";
    FileReader reader;

	output = reader.ReadFile(fileName);
        
    shaderOutput = output.c_str();
    

    //std::cout << "Shader reader output: " << endl << output << endl << endl;

    // load the shaders we grabbed
    // This might have an image param later

    liam_get_shader_program(textureVertex, shaderOutput);

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
    cout << count << endl;
}

void GL_GraphicsEngine::updateTime() {
    if (reverse) progress -= 0.016f;
    else progress += 0.016f;

    if (progress >= 5.0f) {
        progress = 5.0f;
        reverse = true;
    }
    else if (progress <= 0.0f) {
        progress = 0.0f;
        reverse = false;
    }
    currentTime = chrono::high_resolution_clock::now();
    deltaTime = chrono::duration_cast<chrono::duration<float>>(currentTime - startTime).count();
    glUniform1f(timeAttrib, deltaTime);
    glUniform1f(progAttrib, progress / 5.0f);
    //std::cout << progress << endl;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(bigVertices), bigVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // and set our colour uniform to the parsed colours
    //glUniform4f(attribute_colour, r, g, b, a);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // first, we need to assign the new verts to the globally drawn vertices
    glEnableVertexAttribArray(attribute_coord2d);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);
}

void GL_GraphicsEngine::drawRect(GLfloat verts[], GLfloat colour[]) {
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

void GL_GraphicsEngine::drawShader() {
    // First set up vertex elements
    GLuint elements[6] = {
        0, 1, 2,
        2, 3, 0
    };

    // Bind the data in `vertices` to the vbo (vertex buffer object)
    // linked to the GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(bigVertices), bigVertices, GL_STATIC_DRAW);

    // as with the elements
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	
    
    // set the attribute pointers
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	
    
    // enable the attributes
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colAttrib);
	glEnableVertexAttribArray(texAttrib);
	
    // bind the texture
	glBindTexture(GL_TEXTURE_2D, tex);
	
    // draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
    
    // disable the attributes
	glDisableVertexAttribArray(posAttrib);
	glDisableVertexAttribArray(colAttrib);
	glDisableVertexAttribArray(texAttrib);

}

