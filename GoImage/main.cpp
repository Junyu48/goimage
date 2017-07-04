#include "GoBoard.h"

//This is only here so the project will build at all.
int main(int argc, char* argv[])
{
	GoBoard board = GoBoard();
	board.placeStone(TileType::BlackStone, 10, 10);
	board.findEnclosedAreas();
	return 0;
}