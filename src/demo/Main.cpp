
#include "MyGame.h"
#include "LiamGame.h"


int main(int argc, char * args[]) {

	try {
		LiamGame game;
		game.runMainLoop();
	}
	catch (EngineException& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;

}