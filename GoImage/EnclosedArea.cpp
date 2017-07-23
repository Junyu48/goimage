#include "EnclosedArea.h"
#include<iostream>
#include<unordered_set>

EnclosedArea::EnclosedArea(TileType enclosedBy)
	: enclosedBy(enclosedBy)
{
}


EnclosedArea::~EnclosedArea()
{
	for (Tile* t : includedTiles)
	{
		t->setArea(nullptr);
	}
}

void EnclosedArea::assignToArea(Tile* t)
{
	if(std::find(includedTiles.begin(), includedTiles.end(), t) != includedTiles.end())
	{
		return; //this tile is already in this room.
	}
	includedTiles.push_back(t);
	return;
}


EnclosedArea* EnclosedArea::doFloodFill(Tile* t, TileType enclosedBy, std::vector<std::vector<Tile*>> tiles)
{
	EnclosedArea* resultArea = new EnclosedArea(enclosedBy);
	std::queue<Tile*> tilesToEvaluate = std::queue<Tile*>(); //A queue for to hold tiles that may be put in the area.
	std::unordered_set<Tile*> tilesEncountered = std::unordered_set<Tile*>(); //A set to hold tiles that are already in the queue.
	tilesToEvaluate.push(t);                                                  //I need this because you can't search within a queue.
	tilesEncountered.insert(t);
	while(!tilesToEvaluate.empty())
	{
		Tile* tile = tilesToEvaluate.front();
		tilesToEvaluate.pop();

		if (tile->getContents() != Tile::getOppositeColourStone(enclosedBy))
		{ //The tile can't be in an area if it's got a stone on it.
		  //We already checked before enqueueing the tile that it doesn't have the enclosing stone there, so now ensure the opposite one isn't.
			resultArea->assignToArea(tile);
		}
		int x = tile->getX();
		int y = tile->getY();
		Tile* neighbours[4];
		neighbours[0] = tiles[x][y + 1];
		neighbours[1] = tiles[x][y - 1];
		neighbours[2] = tiles[x + 1][y];
		neighbours[3] = tiles[x - 1][y];
		for (int i = 0; i < 4; i++)
		{
			Tile* neighbour = neighbours[i];
			TileType nTT = neighbour->getContents();
			if (nTT == enclosedBy)
			{ //The neighbour is another stone.
				continue;
			}
			if (nTT != TileType::Empty && nTT != Tile::getOppositeColourStone(enclosedBy))
			{//we've run into a wall, and we know from the previous check the tile isn't a stone.
				resultArea->setEncounteredWall(neighbour->getContents());
				continue;
			}
			if (neighbour->getArea() != nullptr && neighbour->getArea()->getEnclosingType() == enclosedBy)
			{ //The neighbour is already in a different room, so we don't need to evaluate it.
				continue;
			}
			if (std::find(resultArea->includedTiles.begin(), resultArea->includedTiles.end(), neighbour) != resultArea->includedTiles.end())
			{ //The neighbour is already in this room, so we don't need to evaluate it.
				continue;
			}
			if (tilesEncountered.find(neighbour) != tilesEncountered.end())
			{ //The neighbour is already in the queue.
				continue;
			}
			tilesToEvaluate.push(neighbour);
			tilesEncountered.insert(neighbour);
		}
	}
	std::vector<Tile*> tilesToRemove = std::vector<Tile*>();
	for (Tile* tile : resultArea->includedTiles)
	{
		if (tile->getArea() == nullptr)
		{
			tile->setArea(resultArea);
		}
		else if(tile->getArea()->getNumTilesEnclosed() > resultArea->getNumTilesEnclosed())
		{
			tile->getArea()->removeTileFromArea(tile);
			tile->setArea(resultArea);
		}
		else
		{
			tilesToRemove.push_back(tile);
		}
	}
	for (Tile* tile : tilesToRemove)
	{
		resultArea->removeTileFromArea(tile);
	}
	if (resultArea->encounteredWalls.size() >= 3)
	{
		resultArea->enclosedBy = TileType::Empty; //The area does not belong to either colour.
	}
	if (resultArea->getNumTilesEnclosed() == 0)
	{
		resultArea = nullptr;
	}
	return resultArea;
}

int EnclosedArea::getNumTilesEnclosed()
{
	return static_cast<int>(includedTiles.size());
}

void EnclosedArea::setEncounteredWall(TileType wall)
{
	if (wall == TileType::Empty || wall == TileType::WhiteStone || wall == TileType::BlackStone)
	{ //only recognize walls, not stones or empty tiles.
		return;
	}
	if (std::find(encounteredWalls.begin(), encounteredWalls.end(), wall) == encounteredWalls.end())
	{ //we have not yet encountered a particular wall, so add it to the walls encountered.
		encounteredWalls.push_back(wall);
	}
	return;
}

TileType EnclosedArea::getEnclosingType()
{
	return enclosedBy;
}

std::vector<Tile*> EnclosedArea::getTilesEnclosed()
{
	return includedTiles;
}

void EnclosedArea::removeTileFromArea(Tile * t)
{
	auto pos = std::find(includedTiles.begin(), includedTiles.end(), t);
	includedTiles.erase(pos);
}
