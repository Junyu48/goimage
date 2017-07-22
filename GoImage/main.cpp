#include "GoBoard.h"

//This is only here so the project will build at all.
int main(int argc, char* argv[])
{
	GoBoard board = GoBoard();
	board.placeStone(TileType::WhiteStone, 10, 10);
	board.placeStone(TileType::WhiteStone, 10, 11);
	board.placeStone(TileType::WhiteStone, 10, 12);
	board.placeStone(TileType::WhiteStone, 11, 13);
	board.placeStone(TileType::WhiteStone, 12, 13);
	board.placeStone(TileType::WhiteStone, 13, 13);
	board.placeStone(TileType::WhiteStone, 14, 12);
	board.placeStone(TileType::WhiteStone, 14, 11);
	board.placeStone(TileType::WhiteStone, 14, 10);
	board.placeStone(TileType::WhiteStone, 13, 9);
	board.placeStone(TileType::WhiteStone, 12, 9);
	board.placeStone(TileType::WhiteStone, 11, 9);
	board.placeStone(TileType::BlackStone, 12, 10);
	board.placeStone(TileType::BlackStone, 12, 12);
	board.placeStone(TileType::BlackStone, 13, 11);
	board.placeStone(TileType::BlackStone, 11, 11);
	board.findEnclosedAreas();
	return 0;
}