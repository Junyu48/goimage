
#pragma once
#include<vector>
#include<iostream>
#include "EnclosedArea.h"

#ifndef GOBOARD_H
#define GOBOARD_H

struct scoreStruct
{
	int whiteScore;
	int blackScore;
	int whiteTotalArea;
	int blackTotalArea;
};

class GoBoard
{
public:
	GoBoard();
	~GoBoard();
	Tile* getTileAt(int x, int y);
	void placeStone(TileType stone, int x, int y);
	Tile** getTileNeighbours(int x, int y);
	void findEnclosedAreas();
	std::vector<EnclosedArea*> getAreas();
	scoreStruct calculateScores();
private:
	std::vector<std::vector<Tile*>> tiles;
	std::vector<EnclosedArea*> areas = std::vector<EnclosedArea*>();
	void findAreasEnclosedBy(TileType stone);
	void removeOutsideAreas();
	static bool isSuitableForFloodFill(Tile* t, TileType stone);
};

#endif