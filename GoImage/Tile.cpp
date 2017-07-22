#include "Tile.h"

Tile::Tile(int x, int y)
	: xPos(x), yPos(y), contents(TileType::Empty)
{
}

Tile::~Tile()
{
}

TileType Tile::getContents()
{
	return contents;
}

void Tile::setContents(TileType newContents)
{
	contents = newContents;
}

int Tile::getX()
{
	return xPos;
}

int Tile::getY()
{
	return yPos;
}

EnclosedArea * Tile::getArea()
{
	return myArea;
}

void Tile::setArea(EnclosedArea * e)
{
	myArea = e;
}

TileType Tile::getOppositeColourStone(TileType stone)
{
	if (stone == TileType::BlackStone)
	{
		return TileType::WhiteStone;
	}
	else if (stone == TileType::WhiteStone)
	{
		return TileType::BlackStone;
	}
	else
	{
		return TileType::Empty;
	}
}
