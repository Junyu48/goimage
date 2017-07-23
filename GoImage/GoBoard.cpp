#include "GoBoard.h"

GoBoard::GoBoard()
{
	tiles = std::vector<std::vector<Tile*>>();
	for (size_t i = 0; i < 21; i++)
	{
		std::vector<Tile*> rowVector = std::vector<Tile*>();
		for (size_t j = 0; j < 21; j++)
		{
			Tile* t = new Tile(static_cast<int>(i), static_cast<int>(j));
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
		std::cout << "Warning: attempted to place stone of invalid type" << "\n";
		return;
	}
	if (x <= 0 || x > 19 || y <= 0 || y > 19) //only allow stone placement within the bounds of the board.
	{
		std::cout << "Warning: attempted to place stone outside of the board at x = " << x << ", y = " << y << "\n";
		return;
	}
	Tile* t = tiles[x][y];
	if (t->getContents() != TileType::Empty)
	{
		std::cout << "Warning: attempted to place stone on top of an already existing stone at x = " << x << ", y = " << y << "\n";
		return;
	}
	else
	{
		t->setContents(stone);
		return;
	}
}

Tile ** GoBoard::getTileNeighbours(int x, int y)
{
	Tile** neighbours = new Tile*[4];
	neighbours[0] = getTileAt(x, y + 1);
	neighbours[1] = getTileAt(x, y - 1);
	neighbours[2] = getTileAt(x + 1, y);
	neighbours[3] = getTileAt(x - 1, y);
	return neighbours;
}

std::vector<EnclosedArea*> GoBoard::getAreas()
{
	return areas;
}

scoreStruct GoBoard::calculateScores()
{
	if (areas.size() == 0)
	{ //the areas have not yet been found
		findEnclosedAreas();
	}
	int whiteNum = 0;
	int blackNum = 0;
	int whiteArea = 0;
	int blackArea = 0;
	for (std::vector<Tile*> yVector : tiles)
	{
		for (Tile* tile : yVector)
		{
			TileType tc = tile->getContents();
			if (tc == TileType::WhiteStone)
			{
				whiteNum++;
			}
			else if (tc == TileType::BlackStone)
			{
				blackNum++;
			}
		}
	}
	for (EnclosedArea* a : areas)
	{
		if (a->getEnclosingType() == TileType::WhiteStone)
		{
			whiteArea += a->getNumTilesEnclosed();
		}
		if (a->getEnclosingType() == TileType::BlackStone)
		{
			blackArea += a->getNumTilesEnclosed();
		}
	}
	return { whiteNum + whiteArea, blackNum + blackArea, whiteArea, blackArea };
}

void GoBoard::findEnclosedAreas()
{
	findAreasEnclosedBy(TileType::WhiteStone);
	removeOutsideAreas();
	findAreasEnclosedBy(TileType::BlackStone);
}

void GoBoard::findAreasEnclosedBy(TileType stone)
{
	for (std::vector<Tile*> yVector : tiles)
	{
		for (Tile* tile : yVector)
		{
			if (tile->getContents() != stone)
			{
				continue;
			}
			Tile* north = getTileAt(tile->getX(), tile->getY() + 1);
			Tile* south = getTileAt(tile->getX(), tile->getY() - 1);
			Tile* east = getTileAt(tile->getX() + 1, tile->getY());
			Tile* west = getTileAt(tile->getX() - 1, tile->getY());
			if (isSuitableForFloodFill(north, stone))
			{
				EnclosedArea* newArea = EnclosedArea::doFloodFill(north, stone, tiles);
				if (newArea != nullptr)
				{
					areas.push_back(newArea);
				}
			}
			if (isSuitableForFloodFill(south, stone))
			{
				EnclosedArea* newArea = EnclosedArea::doFloodFill(south, stone, tiles);
				if (newArea != nullptr)
				{
					areas.push_back(newArea);
				}
			}
			if (isSuitableForFloodFill(east, stone))
			{
				EnclosedArea* newArea = EnclosedArea::doFloodFill(east, stone, tiles);
				if (newArea != nullptr)
				{
					areas.push_back(newArea);
				}
			}
			if (isSuitableForFloodFill(west, stone))
			{
				EnclosedArea* newArea = EnclosedArea::doFloodFill(west, stone, tiles);
				if (newArea != nullptr)
				{
					areas.push_back(newArea);
				}
			}
		}
	}
}

void GoBoard::removeOutsideAreas()
{
	std::vector<EnclosedArea*> ousideAreas = std::vector<EnclosedArea*>();
	for (EnclosedArea* e : areas)
	{
		if (e->getEnclosingType() == TileType::Empty)
		{
			ousideAreas.push_back(e);
		}
	}
	for (EnclosedArea* e : ousideAreas)
	{
		auto pos = std::find(areas.begin(), areas.end(), e);
		areas.erase(pos);
		delete e;
	}
}

bool GoBoard::isSuitableForFloodFill(Tile * t, TileType stone)
{
	return (t->getArea() == nullptr || t->getArea()->getEnclosingType() == Tile::getOppositeColourStone(stone))
		&& (t->getContents() == TileType::Empty || t->getContents() == Tile::getOppositeColourStone(stone));
}
