#pragma once

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

private:
	TileType contents;
	const int xPos;
	const int yPos;

};

