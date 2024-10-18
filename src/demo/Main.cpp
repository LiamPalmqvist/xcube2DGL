#include "MyGame.h"
#include "LiamGame.h"
#include "FileReader.h"
#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>


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