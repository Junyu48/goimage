#include<vector>;
#include<iostream>;
#include "Tile.h";

#pragma once
class GoBoard
{
public:
	GoBoard();
	~GoBoard();
	Tile* getTileAt(int x, int y);
	void placeStone(TileType stone, int x, int y);
private:
	std::vector<std::vector<Tile*>> tiles;
};

