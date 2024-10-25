
#include "MyGame.h"
#include "LiamGame.h"


int main(int argc, char * args[]) {

	/*
	try {
        MyGame game;
		game.runMainLoop();
	} catch (EngineException & e) {
		std::cout << e.what() << std::endl;
		getchar();
	}
	*/

	/*try {
		FileReader reader;
		string fileName ("shader.fragment");
		reader.ReadFile(fileName);
		reader.WriteFile();
	}
	catch(exception e) {
		std::cout << e.what() << std::endl;
	}*/

	try {
		LiamGame game;
		game.runMainLoop();
	}
	catch (EngineException& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}

//#include <SDL.h>
//#include <SDL_image.h>
//#include <GL/glew.h>
//#include <GL/GL.h>
//#include <iostream>
//
//const GLchar* vertexSource = R"glsl(
//    #version 150 core
//    in vec2 position;
//    in vec3 color;
//    in vec2 texcoord;
//    out vec3 Color;
//    out vec2 Texcoord;
//    void main()
//    {
//        Color = color;
//        Texcoord = texcoord;
//        gl_Position = vec4(position, 0.0, 1.0);
//    }
//)glsl";
//const GLchar* fragmentSource = R"glsl(
//    #version 150 core
//    in vec3 Color;
//    in vec2 Texcoord;
//    out vec4 outColor;
//    uniform sampler2D tex;
//    void main()
//    {
//        outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
//    }
//)glsl";
//
//int main(int argc, char* args[])
//{
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
//        throw ("SDL_Init()", SDL_GetError());
//
//    //Uint32 ticks = SDL_GetTicks();
//
//    SDL_Window * window = SDL_CreateWindow(
//        "The X-CUBE 2D Game Engine - OpenGL Version",
//        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//        800, 600,
//        SDL_WINDOW_OPENGL
//    );
//
//    // Checking if the window has been created or not
//    if (nullptr == window) {
//        throw ("Failed to create window", SDL_GetError());
//	}
//	else {
//		std::cout << "Created window" << std::endl;
//	}
//
//    // Initialize GLEW
//    glewExperimental = GL_TRUE;
//    SDL_GLContext glContext = SDL_GL_CreateContext(window);
//    glewInit();
//
//    if (nullptr == glContext) {
//        std::cout << "Failed to create OpenGL context" << std::endl;
//        throw ("Failed to create OpenGL context", SDL_GetError());
//    }
//    else {
//		std::cout << "Created OpenGL context" << std::endl;
//    }
//
//    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
//		std::cout << "Failed to init SDL_image - PNG" << std::endl;
//        throw ("Failed to init SDL_image - PNG", IMG_GetError());
//	}
//	else {
//		std::cout << "Initialized SDL_image - PNG" << std::endl;
//	}
//
//
//
//    // Create Vertex Array Object
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//	std::cout << "Created vertex array object" << std::endl;
//
//    // Create a Vertex Buffer Object and copy the vertex data to it
//    GLuint vbo;
//    glGenBuffers(1, &vbo);
//	std::cout << "Created vertex buffer object" << std::endl;
//
//    /*
//    =====================================================================
//    ==================Probably something to do with this=================
//    =====================================================================
//    */
//    GLfloat vertices[] = {
//        //  Position      Color             Texcoords
//        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
//         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
//         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
//        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
//    };
//
//    /*
//    =====================================================================
//    =====================================================================
//    =====================================================================
//    */
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Create an element array
//    GLuint ebo;
//    glGenBuffers(1, &ebo);
//
//    GLuint elements[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//	std::cout << "Created element array" << std::endl;
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
//
//    // Create and compile the vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Create and compile the fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Link the vertex and fragment shader into a shader program
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glBindFragDataLocation(shaderProgram, 0, "outColor");
//    glLinkProgram(shaderProgram);
//    glUseProgram(shaderProgram);
//
//    // Specify the layout of the vertex data
//    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
//    glEnableVertexAttribArray(posAttrib);
//    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
//
//    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
//    glEnableVertexAttribArray(colAttrib);
//    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
//
//    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
//    glEnableVertexAttribArray(texAttrib);
//    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
//
//    // Load texture
//    GLuint tex;
//    glGenTextures(1, &tex);
//    glBindTexture(GL_TEXTURE_2D, tex);
//
//	SDL_Surface* surface = IMG_Load("sample.png");
//    if (surface == nullptr) {
//        std::cout << "Failed to load image" << std::endl;
//    }
//    else {
//        std::cout << "Loaded image" << std::endl;
//    }
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
//    SDL_free(surface);
//
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    bool running = true;
//    while (running)
//    {
//        // Clear the screen to black
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Draw a rectangle from the 2 triangles using 6 indices
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//        // Swap buffers
//        SDL_GL_SwapWindow(window);
//    }
//
//    glDeleteTextures(1, &tex);
//
//    glDeleteProgram(shaderProgram);
//    glDeleteShader(fragmentShader);
//    glDeleteShader(vertexShader);
//
//    glDeleteBuffers(1, &ebo);
//    glDeleteBuffers(1, &vbo);
//
//    glDeleteVertexArrays(1, &vao);
//
//    SDL_GL_DeleteContext(glContext);
//	
//
//    return 0;
//}