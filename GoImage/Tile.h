#pragma once

#ifndef TILE_H
#define TILE_H

class EnclosedArea;
enum class TileType
{
	Empty,
	WhiteStone,
	BlackStone,
	NorthWall,
	EastWall,
	SouthWall,
	WestWall
};

class Tile
{
public:
	Tile(int x, int y);
	~Tile();
	TileType getContents();
	void setContents(TileType newContents);
	int getX();
	int getY();
	EnclosedArea* getArea();
	void setArea(EnclosedArea* e);
	static TileType getOppositeColourStone(TileType stone);
private:
	TileType contents;
	const int xPos;
	const int yPos;
	EnclosedArea* myArea = nullptr;
};

#endif
