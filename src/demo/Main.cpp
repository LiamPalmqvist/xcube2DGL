

#include "LiamGame.h"


int main(int argc, char * args[]) {

	try {
		// LiamGame is a modification of MyGame.cpp/.h
		LiamGame game;
		game.runMainLoop();
	}
	catch (EngineException& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;

}