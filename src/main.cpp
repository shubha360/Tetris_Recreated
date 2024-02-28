#include "../include/Tetris.h"

int main(int args, char** argv) {
	Tetris tetrisGame;
	if (tetrisGame.init()) {
		tetrisGame.run();
	}
	return 0;
}