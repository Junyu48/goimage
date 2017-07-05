#pragma once
#include "Tile.h"
#include<vector>
#include<queue>

#ifndef ENCLOSEDAREA_H
#define ENCLOSEDAREA_H

class EnclosedArea
{
public:
	EnclosedArea(TileType enclosedBy);
	~EnclosedArea();

	void assignToArea(Tile* t);
	std::vector<Tile*> getTilesEnclosed();

	static EnclosedArea* doFloodFill(Tile* t, TileType enclosedBy, std::vector<std::vector<Tile*>> tiles);
	int getNumTilesEnclosed();
	void setEncounteredWall(TileType wall);

	TileType getEnclosingType();

private:
	std::vector<Tile*> includedTiles = std::vector<Tile*>();
	std::vector<TileType> encounteredWalls = std::vector<TileType>();
	TileType enclosedBy; //Either a stone colour, or Empty.

	void removeTileFromArea(Tile* t);
};

#endif