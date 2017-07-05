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
		
		TEST_METHOD(TestBoard_Initialization)
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
					Assert::AreEqual(i, t.getX(), L"Wrong X position!");
					Assert::AreEqual(j, t.getY(), L"Wrong Y position!");
					Assert::AreEqual(ttInt, static_cast<int>(t.getContents()), L"Wrong tile contents!");
					Assert::AreEqual(0, static_cast<int>(board.getAreas().size()), L"Should have no areas!");
				}
			}
		}
		TEST_METHOD(TestBoard_PlaceStone)
		{
			GoBoard board = GoBoard();
			Tile* t = board.getTileAt(10, 10);
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(t->getContents()), L"Wrong tile contents!");
			board.placeStone(TileType::BlackStone, 10, 10);
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(t->getContents()), L"Wrong tile contents!");
		}
	};
	TEST_CLASS(EnclosedAreaTests)
	{
	public:
		TEST_METHOD(TestFindAreas_OneStone)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(1, static_cast<int>(areas.size()), L"Should have exactly one area!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 1, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the one with a black stone and the walls.");
		}
		TEST_METHOD(TestFindAreas_OneWhiteOneBlack)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::WhiteStone, 11, 11);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(1, static_cast<int>(areas.size()), L"Should have exactly one area!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 2, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones and the walls.");
		}
		TEST_METHOD(TestFindAreas_TwoAreas)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 12);
			board.placeStone(TileType::BlackStone, 11, 11);
			board.placeStone(TileType::BlackStone, 9, 11);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(2, static_cast<int>(areas.size()), L"Should have two areas!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 5, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the black stones, the walls, and the one tile surrounded by the black stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(1, areas[1]->getNumTilesEnclosed(), L"The area should have one tile.");
		}
		TEST_METHOD(TestFindAreas_AreaWithDifferentColouredBorders)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 11);
			board.placeStone(TileType::BlackStone, 11, 12);
			board.placeStone(TileType::BlackStone, 12, 12);
			board.placeStone(TileType::WhiteStone, 13, 11);
			board.placeStone(TileType::BlackStone, 13, 10);
			board.placeStone(TileType::BlackStone, 12, 9);
			board.placeStone(TileType::BlackStone, 11, 9);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(1, static_cast<int>(areas.size()), L"Should have exactly one area!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 8, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones and the walls.");
		}
		TEST_METHOD(TestFindAreas_TwoAreas_BiggerArea)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 11);
			board.placeStone(TileType::BlackStone, 11, 12);
			board.placeStone(TileType::BlackStone, 12, 12);
			board.placeStone(TileType::BlackStone, 13, 11);
			board.placeStone(TileType::BlackStone, 13, 10);
			board.placeStone(TileType::BlackStone, 12, 9);
			board.placeStone(TileType::BlackStone, 11, 9);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(2, static_cast<int>(areas.size()), L"Should have two areas!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 12, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the black stones, the walls, and the tiles surrounded by the black stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(4, areas[1]->getNumTilesEnclosed(), L"The area should have three tiles.");
		}
		TEST_METHOD(TestFindAreas_TwoAreas_BiggerArea_WithWhiteStoneInside)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 11);
			board.placeStone(TileType::BlackStone, 11, 12);
			board.placeStone(TileType::BlackStone, 12, 12);
			board.placeStone(TileType::BlackStone, 13, 11);
			board.placeStone(TileType::BlackStone, 13, 10);
			board.placeStone(TileType::BlackStone, 12, 9);
			board.placeStone(TileType::BlackStone, 11, 9);
			board.placeStone(TileType::WhiteStone, 11, 11);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(2, static_cast<int>(areas.size()), L"Should have two areas!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 12, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones, the walls, and the tiles surrounded by the black stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(3, areas[1]->getNumTilesEnclosed(), L"The area should have three tiles.");
		}
		TEST_METHOD(TestFindAreas_TwoAreas_BiggerArea_WithBlackStoneInside)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::WhiteStone, 10, 10);
			board.placeStone(TileType::WhiteStone, 10, 11);
			board.placeStone(TileType::WhiteStone, 11, 12);
			board.placeStone(TileType::WhiteStone, 12, 12);
			board.placeStone(TileType::WhiteStone, 13, 11);
			board.placeStone(TileType::WhiteStone, 13, 10);
			board.placeStone(TileType::WhiteStone, 12, 9);
			board.placeStone(TileType::WhiteStone, 11, 9);
			board.placeStone(TileType::BlackStone, 11, 11);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(2, static_cast<int>(areas.size()), L"Should have two areas!");
			Assert::AreEqual(static_cast<int>(TileType::WhiteStone), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by white stones!");
			Assert::AreEqual(3, areas[0]->getNumTilesEnclosed(), L"The area should have three tiles.");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 12, areas[1]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones, the walls, and the tiles surrounded by the white stones.");

		}
		TEST_METHOD(TestFindAreas_TwoAreas_DifferentStones)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 12);
			board.placeStone(TileType::BlackStone, 11, 11);
			board.placeStone(TileType::BlackStone, 9, 11);
			board.placeStone(TileType::WhiteStone, 13, 13);
			board.placeStone(TileType::WhiteStone, 13, 15);
			board.placeStone(TileType::WhiteStone, 14, 14);
			board.placeStone(TileType::WhiteStone, 12, 14);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(3, static_cast<int>(areas.size()), L"Should have three areas!");
			Assert::AreEqual(static_cast<int>(TileType::WhiteStone), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by white stones!");
			Assert::AreEqual(1, areas[0]->getNumTilesEnclosed(), L"The area should have one tile.");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 10, areas[1]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones, the walls, and the tiles surrounded by stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[2]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(1, areas[2]->getNumTilesEnclosed(), L"The area should have one tile.");
		}
		TEST_METHOD(TestFindAreas_AreaUsingBorders)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 1, 2);
			board.placeStone(TileType::BlackStone, 2, 1);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(2, static_cast<int>(areas.size()), L"Should have two areas!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 3, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the black stones, the walls, and the one tile surrounded by the black stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(1, areas[1]->getNumTilesEnclosed(), L"The area should have one tile.");
		}
		TEST_METHOD(TestFindAreas_AreaUsingBorders_Bigger)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 3, 1);
			board.placeStone(TileType::BlackStone, 2, 2);
			board.placeStone(TileType::BlackStone, 1, 3);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(2, static_cast<int>(areas.size()), L"Should have two areas!");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 6, areas[0]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the black stones, the walls, and the one tile surrounded by the black stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(3, areas[1]->getNumTilesEnclosed(), L"The area should have one tile.");
		}
		TEST_METHOD(TestFindAreas_TwoAreas_DifferentStones_OneAreaInsideAnother)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 11);
			board.placeStone(TileType::BlackStone, 10, 12);
			board.placeStone(TileType::BlackStone, 11, 13);
			board.placeStone(TileType::BlackStone, 12, 13);
			board.placeStone(TileType::BlackStone, 13, 13);
			board.placeStone(TileType::BlackStone, 14, 12);
			board.placeStone(TileType::BlackStone, 14, 11);
			board.placeStone(TileType::BlackStone, 14, 10);
			board.placeStone(TileType::BlackStone, 13, 9);
			board.placeStone(TileType::BlackStone, 12, 9);
			board.placeStone(TileType::BlackStone, 11, 9);
			board.placeStone(TileType::WhiteStone, 12, 10);
			board.placeStone(TileType::WhiteStone, 12, 12);
			board.placeStone(TileType::WhiteStone, 13, 11);
			board.placeStone(TileType::WhiteStone, 11, 11);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(3, static_cast<int>(areas.size()), L"Should have three areas!");
			Assert::AreEqual(static_cast<int>(TileType::WhiteStone), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by white stones!");
			Assert::AreEqual(1, areas[0]->getNumTilesEnclosed(), L"The area should have one tile.");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 21, areas[1]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones, the walls, and the tiles surrounded by stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[2]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(4, areas[2]->getNumTilesEnclosed(), L"The area should have four tiles.");
		}
		TEST_METHOD(TestFindAreas_TwoAreas_DifferentStones_OneAreaInsideAnother_InvertedColours)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::WhiteStone, 10, 10);
			board.placeStone(TileType::WhiteStone, 10, 11);
			board.placeStone(TileType::WhiteStone, 10, 12);
			board.placeStone(TileType::WhiteStone, 11, 13);
			board.placeStone(TileType::WhiteStone, 12, 13);
			board.placeStone(TileType::WhiteStone, 13, 13);
			board.placeStone(TileType::WhiteStone, 14, 12);
			board.placeStone(TileType::WhiteStone, 14, 11);
			board.placeStone(TileType::WhiteStone, 14, 10);
			board.placeStone(TileType::WhiteStone, 13, 9);
			board.placeStone(TileType::WhiteStone, 12, 9);
			board.placeStone(TileType::WhiteStone, 11, 9);
			board.placeStone(TileType::BlackStone, 12, 10);
			board.placeStone(TileType::BlackStone, 12, 12);
			board.placeStone(TileType::BlackStone, 13, 11);
			board.placeStone(TileType::BlackStone, 11, 11);
			board.findEnclosedAreas();
			auto areas = board.getAreas();
			Assert::AreEqual(3, static_cast<int>(areas.size()), L"Should have three areas!");
			Assert::AreEqual(static_cast<int>(TileType::WhiteStone), static_cast<int>(areas[0]->getEnclosingType()), L"Area should be enclosed by white stones!");
			Assert::AreEqual(4, areas[0]->getNumTilesEnclosed(), L"The area should have one tile.");
			Assert::AreEqual(static_cast<int>(TileType::Empty), static_cast<int>(areas[1]->getEnclosingType()), L"Area should be enclosed by nothing!");
			Assert::AreEqual((19 * 19) - 21, areas[1]->getNumTilesEnclosed(), L"The area should have every tile except the ones with the stones, the walls, and the tiles surrounded by stones.");
			Assert::AreEqual(static_cast<int>(TileType::BlackStone), static_cast<int>(areas[2]->getEnclosingType()), L"Area should be enclosed by black stones!");
			Assert::AreEqual(1, areas[2]->getNumTilesEnclosed(), L"The area should have four tiles.");
		}
	};

	TEST_CLASS(ScoreTests)
	{
	public:

		TEST_METHOD(TestFindAreas_TwoAreas_DifferentStones)
		{
			GoBoard board = GoBoard();
			board.placeStone(TileType::BlackStone, 10, 10);
			board.placeStone(TileType::BlackStone, 10, 12);
			board.placeStone(TileType::BlackStone, 11, 11);
			board.placeStone(TileType::BlackStone, 9, 11);
			board.placeStone(TileType::WhiteStone, 13, 13);
			board.placeStone(TileType::WhiteStone, 13, 15);
			board.placeStone(TileType::WhiteStone, 14, 14);
			board.placeStone(TileType::WhiteStone, 12, 14);
			board.placeStone(TileType::WhiteStone, 1, 14);
			scoreStruct scores = board.calculateScores();
			Assert::AreEqual(5, scores.blackScore, L"Black should have 5 points!");
			Assert::AreEqual(6, scores.whiteScore, L"White should have 6 points!");
			Assert::AreEqual(1, scores.blackTotalArea, L"Black should be enclosing 1 tile!");
			Assert::AreEqual(1, scores.whiteTotalArea, L"White should be enclosing 1 tile!");
		}
	};
}