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
