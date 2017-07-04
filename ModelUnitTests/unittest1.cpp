#include "stdafx.h"
#include "CppUnitTest.h"
#include "../GoImage/GoBoard.cpp"
#include "../GoImage/Tile.cpp"
#include "../GoImage/EnclosedArea.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ModelUnitTests
{		
	TEST_CLASS(BoardTests)
	{
	public:
		
		TEST_METHOD(TestBoardInitialization)
		{
			GoBoard board = GoBoard();
			for (int i = 0; i < 21; i++)
			{
				for (int j = 0; j < 21; j++)
				{
					Tile t = *board.getTileAt(i, j);
					TileType tt = TileType::Empty;
					if (i == 0)
					{
						tt = TileType::WestWall;
					}
					else if (i == 20)
					{
						tt = TileType::EastWall;
					}
					else if (j == 0)
					{
						tt = TileType::NorthWall;
					}
					else if (j == 20)
					{
						tt = TileType::SouthWall;
					}
					int ttInt = static_cast<int>(tt);
					Assert::AreEqual(t.getX(), i, L"Wrong X position!");
					Assert::AreEqual(t.getY(), j, L"Wrong Y position!");
					Assert::AreEqual(static_cast<int>(t.getContents()), ttInt, L"Wrong tile contents!");
					Assert::AreEqual(static_cast<int>(board.getAreas().size()), 0, L"Should have no areas!");
				}
			}
		}
		TEST_METHOD(TestBoardPlaceStone)
		{
			GoBoard board = GoBoard();
			Tile* t = board.getTileAt(10, 10);
			Assert::AreEqual(static_cast<int>(t->getContents()), static_cast<int>(TileType::Empty), L"Wrong tile contents!");
			board.placeStone(TileType::BlackStone, 10, 10);
			Assert::AreEqual(static_cast<int>(t->getContents()), static_cast<int>(TileType::BlackStone), L"Wrong tile contents!");
		}
		TEST_METHOD(TestBoardFindAreas)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(1, 1);
			//Assert::AreEqual(static_cast<int>(areas.size()), 1, L"Should have exactly one area!");
			//Assert::AreEqual(static_cast<int>(areas[0]->getEnclosingType()), static_cast<int>(TileType::Empty), L"Area should be enclosed by nothing!");
		}
	};
}