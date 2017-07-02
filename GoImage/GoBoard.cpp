#include "GoBoard.h"



GoBoard::GoBoard()
{
	tiles = std::vector<std::vector<Tile*>>();
	for (size_t i = 0; i < 21; i++)
	{
		std::vector<Tile*> rowVector = std::vector<Tile*>();
		for (size_t j = 0; j < 21; j++)
		{
			Tile* t = new Tile(i, j);
			if (i == 0)
			{
				t->setContents(TileType::WestWall);
			}
			else if (i == 20)
			{
				t->setContents(TileType::EastWall);
			}
			else if (j == 0)
			{
				t->setContents(TileType::NorthWall);
			}
			else if (j == 20)
			{
				t->setContents(TileType::SouthWall);
			}
			rowVector.push_back(t);
		}
		tiles.push_back(rowVector);
	}
}


GoBoard::~GoBoard()
{
}

Tile* GoBoard::getTileAt(int x, int y)
{
	if (x > 20 || y > 20)
	{
		return nullptr;
	}
	else
	{
		return tiles[x][y];

	}
}

void GoBoard::placeStone(TileType stone, int x, int y)
{
	if (stone != TileType::BlackStone && stone != TileType::WhiteStone)
	{
		std::cout << "Warning: attempted to place stone of invalid type";
		return;
	}
	if (x <= 0 || x >= 19 || y <= 0 || y >= 19) //only allow stone placement within the bounds of the board.
	{
		std::cout << "Warning: attempted to place stone outside of the board";
		return;
	}
	Tile* t = tiles[x][y];
	if (t->getContents() != TileType::Empty)
	{
		std::cout << "Warning: attempted to place stone on top of an already existing stone";
		return;
	}
	else
	{
		t->setContents(stone);
		return;
	}
}
