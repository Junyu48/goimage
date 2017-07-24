#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "GoBoard.h"
#include "Tile.h"

//#define USE_EXISTING_IMG
//#define SAVE_IMG
//#define DEBUG

using namespace cv;
using namespace std;

Mat frame;
vector<double> ip_x;
vector<double> ip_y;

//initialize board model
GoBoard board = GoBoard();

struct gridTile
{
	Point ip;
	TileType stone;
};

// dynamically detect the grid and draw it on the empty board
void detectGrid(Mat src, Mat dst)
{
	cvtColor(src, dst, CV_BGR2GRAY);
	Canny(dst, dst, 100, 200, 3);
	imshow("edge", dst);

	vector<Vec2f> lines;
	// detect lines
	HoughLines(dst, lines, 1, CV_PI / 180, 180, 0, 0);

	Point pt1, pt2;
	vector<double> p_x, p_y;
	// draw lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		if (theta == 0)
		{
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			p_x.push_back(x0);
		}
		if (theta > CV_PI / 180 * 85 && theta < CV_PI / 180 * 95)
		{
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			p_y.push_back(y0);
		}
	}

	sort(p_x.begin(), p_x.end());
	double previous_x = -15; // initial value, this will make sure the first line is registered
	for (int i = 0; i < p_x.size(); i++)
	{
		if ((p_x[i] - previous_x) > 10)
		{
			Point pt1, pt2;
			pt1.x = static_cast<int>(p_x[i]);
			pt1.y = 1000;
			pt2.x = static_cast<int>(p_x[i]);
			pt2.y = -1000;
			ip_x.push_back(p_x[i]);
		}
		previous_x = p_x[i];
	}

	while (ip_x.size() > 19)
	{
		if (abs(ip_x[0] - (src.cols / 2)) > abs(ip_x[ip_x.size() - 1] - (src.cols / 2)))
		{
			ip_x.erase(ip_x.begin());
		}
		else
		{
			ip_x.pop_back();
		}
	}
	for (int i = 0; i < ip_x.size(); i++)
	{
		Point pt1, pt2;
		pt1.x = static_cast<int>(ip_x[i]);
		pt1.y = 1000;
		pt2.x = static_cast<int>(ip_x[i]);
		pt2.y = -1000;
		line(frame, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA);
	}

	sort(p_y.begin(), p_y.end());
	double previous_y = -15; // initial value, this will make sure the first line is registered
	for (int i = 0; i < p_y.size(); i++)
	{
		if ((p_y[i] - previous_y) > 10)
		{
			Point pt1, pt2;
			pt1.x = -1000;
			pt1.y = static_cast<int>(p_y[i]);
			pt2.x = 1000;
			pt2.y = static_cast<int>(p_y[i]);
			ip_y.push_back(p_y[i]);
		}
		previous_y = p_y[i];
	}

	while (ip_y.size() > 19)
	{
		if (abs(ip_y[0] - (src.rows / 2)) > abs(ip_y[ip_y.size() - 1] - (src.rows / 2)))
		{
			ip_y.erase(ip_y.begin());
		}
		else
		{
			ip_y.pop_back();
		}
	}
	for (int i = 0; i < ip_y.size(); i++)
	{
		Point pt1, pt2;
		pt1.x = -1000;
		pt1.y = static_cast<int>(ip_y[i]);
		pt2.x = 1000;
		pt2.y = static_cast<int>(ip_y[i]);
		line(frame, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA);
	}
}

//detect stones and popultate gridTile vector
void detectStones(Mat fgMaskMOG2, Mat finishedFrame)
{
	// adaptive thresholding on the final board to remove lighting effect for white stone detection
	// can't use this to detect black stones because thresholded black stones have white circles caused by illumination on them
	Mat bFinishedFrame = finishedFrame.clone();
	cvtColor(bFinishedFrame, bFinishedFrame, CV_BGR2GRAY);
	adaptiveThreshold(bFinishedFrame, bFinishedFrame, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11, 12);
	imshow("thresholded image", bFinishedFrame);

	for (int i = 0; i < ip_x.size(); i++)
	{
		for (int j = 0; j < ip_y.size(); j++)
		{
			int xBoundLeft = static_cast<int>(ip_x[i] - 3);
			int xBoundRight = static_cast<int>(ip_x[i] + 3);
			int yBoundUp = static_cast<int>(ip_y[j] - 3);
			int yBoundDown = static_cast<int>(ip_y[j] + 3);
			int mean = 0;
			for (int x = xBoundLeft; x < xBoundRight; x++)
			{
				for (int y = yBoundUp; y < yBoundDown; y++)
				{
					if (y < 0 || y > fgMaskMOG2.rows || x < 0 || x > fgMaskMOG2.cols)
					{
						continue;
					}
					mean += fgMaskMOG2.at<uchar>(y, x);
				}
			}
			mean /= 49;

#ifdef DEBUG
			cout << "the gray mean at x = " << i << ", y = " << j << "is " << mean << endl;
#endif // DEBUG

			if (mean > 25)
			{
				int blueMean = 0;
				int greenMean = 0;
				int redMean = 0;
				for (int x = xBoundLeft - 5; x < xBoundRight + 5; x++)
				{
					for (int y = yBoundUp - 5; y < yBoundDown + 5; y++)
					{
						if (y < 0 || y > fgMaskMOG2.rows || x < 0 || x > fgMaskMOG2.cols)
						{
							continue;
						}
						Vec3b intensity = finishedFrame.at<Vec3b>(y, x);
						blueMean += intensity.val[0];
						greenMean += intensity.val[1];
						redMean += intensity.val[2];
					}
				}
				blueMean /= 289;
				greenMean /= 289;
				redMean /= 289;

#ifdef DEBUG
				cout << "the BGR mean at x = " << i << ", y = " << j << "is " << blueMean << "," << greenMean << "," << redMean << endl;
#endif // DEBUG

				if (blueMean < 100 && greenMean < 100 && redMean < 100)
				{
					board.placeStone(TileType::BlackStone, i + 1, j + 1);
				}
				else /* if (blueMean > 100 && greenMean > 100 && redMean > 100 && (redMean - (blueMean+greenMean)/2 < 10))*/
				{
					int blackPixels = 0;
					for (int x = xBoundLeft; x < xBoundRight; x++)
					{
						for (int y = yBoundUp; y < yBoundDown; y++)
						{
							if (y < 0 || y > bFinishedFrame.rows || x < 0 || x > bFinishedFrame.cols)
							{
								continue;
							}
							uchar c = bFinishedFrame.at<uchar>(y, x);
							// since it's a binary image, the threshold will either be 0 or 255
							if (c < 100)
							{
								blackPixels++;
							}
						}
					}

#ifdef DEBUG
					cout << "the black pixels at x = " << i << ", y = " << j << "is " << blackPixels << endl;
#endif // DEBUG

					if (blackPixels <= 5)
					{
						board.placeStone(TileType::WhiteStone, i + 1, j + 1);
					}
				}
			}
		}
	}
}

int main()
{
	Mat bgframe;

#ifndef USE_EXISTING_IMG
	VideoCapture cap;
	if (!cap.open(0)) return 0;
#endif // !USE_EXISTING_IMG

	for (;;)
	{
		ip_x.clear();
		ip_y.clear();

#ifndef USE_EXISTING_IMG
		cap >> bgframe;
		if (bgframe.empty()) break; // end of video stream
#endif // !USE_EXISTING_IMG

#ifdef USE_EXISTING_IMG
		bgframe = imread("emptyboard5.jpg");
#endif // USE_EXISTING_IMG

		frame = bgframe.clone();

#ifdef SAVE_IMG
		imwrite("emptyboard.bmp", frame);
#endif // SAVE_IMG

		Mat src = frame.clone();
		Mat dst;

		//display crosshair for placement assistance
		line(frame, Point(frame.cols / 2 + 20, frame.rows / 2), Point(frame.cols / 2 - 20, frame.rows / 2), Scalar(0, 0, 255));
		line(frame, Point(frame.cols / 2, frame.rows / 2 + 20), Point(frame.cols / 2, frame.rows / 2 - 20), Scalar(0, 0, 255));

		detectGrid(src, dst);

		imshow("Detecting Grid", frame);
		if (waitKey(10) == 27) break; // stop capturing by pressing ESC 
	}

	if (ip_x.size() != 19 || ip_y.size() != 19)
	{
		cout << "Error detecting grid, please try again, the program will terminate shortly" << endl;
		destroyAllWindows();
		// wait for 3 seconds, then exit
		Sleep(3000);
		return 0;
	}

	Mat finishedFrame;

#ifndef USE_EXISTING_IMG
	for (;;)
	{
		cap >> finishedFrame;
		if (finishedFrame.empty()) break; // end of video stream

		Mat temp = finishedFrame.clone();
		addWeighted(temp, 0.5, bgframe, 0.5, 0, temp);
		imshow("Final Board", temp);
		if (waitKey(10) == 27) break; // stop capturing by pressing ESC 
	}
#endif // !USE_EXISTING_IMG

#ifdef USE_EXISTING_IMG
	finishedFrame = imread("stones3.jpg");
#endif // USE_EXISTING_IMG

#ifdef SAVE_IMG
	imwrite("stones.bmp", finishedFrame);
#endif // SAVE_IMG

	Mat frame1, frame2; //current frame
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG method
	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor

	pMOG2 = createBackgroundSubtractorMOG2();

	frame1 = bgframe.clone();
	cvtColor(frame1, frame1, CV_BGR2GRAY);
	GaussianBlur(frame1, frame1, Size(5, 5), 0, 0);

#ifdef DEBUG
	imshow("Empty Board Gray", frame1);
#endif // DEBUG
	imshow("Empty Board", bgframe);

	frame2 = finishedFrame.clone();
	cvtColor(frame2, frame2, CV_BGR2GRAY);
	GaussianBlur(frame2, frame2, Size(5, 5), 0, 0);

#ifdef DEBUG
	imshow("Final Board Gray", frame2);
#endif // DEBUG
	imshow("Final Board", finishedFrame);

	//update the background model
	pMOG2->apply(frame1, fgMaskMOG2);
	//update the background model
	pMOG2->apply(frame2, fgMaskMOG2);

	medianBlur(fgMaskMOG2, fgMaskMOG2, 5);

	Mat displayMask = fgMaskMOG2.clone();

#ifdef DEBUG
	cout << ip_x.size() << " ," << ip_y.size() << endl;
#endif // DEBUG

	for (int i = 0; i < 19; i++)
	{
		Point pt1, pt2;
		pt1.x = static_cast<int>(ip_x[i]);
		pt1.y = 1000;
		pt2.x = static_cast<int>(ip_x[i]);
		pt2.y = -1000;
		line(displayMask, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA);
		pt1.x = -1000;
		pt1.y = static_cast<int>(ip_y[i]);
		pt2.x = 1000;
		pt2.y = static_cast<int>(ip_y[i]);
		line(displayMask, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA);
	}

	imshow("FG Mask MOG2", displayMask);

	detectStones(fgMaskMOG2, finishedFrame);

	//Display detected stones
	Mat displayFrame = bgframe.clone();

	// draw the grid
	for (int i = 0; i < 19; i++)
	{
		Point pt1, pt2;
		pt1.x = static_cast<int>(ip_x[i]);
		pt1.y = 1000;
		pt2.x = static_cast<int>(ip_x[i]);
		pt2.y = -1000;
		line(displayFrame, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA);
		pt1.x = -1000;
		pt1.y = static_cast<int>(ip_y[i]);
		pt2.x = 1000;
		pt2.y = static_cast<int>(ip_y[i]);
		line(displayFrame, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA);
	}

	// draw stones
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			TileType stone = board.getTileAt(i + 1, j + 1)->getContents();
			if (stone == TileType::WhiteStone || stone == TileType::BlackStone)
			{
				Scalar colour = Scalar(0, 0, 0);
				if (stone == TileType::WhiteStone)
				{
					colour = Scalar(255, 255, 255);
				}
				circle(displayFrame, Point(static_cast<int>(ip_x[i]), static_cast<int>(ip_y[j])), 10, colour, -1);
			}

		}
	}
	imshow("Detected stones", displayFrame);

	auto scores = board.calculateScores();
	cout << "Black scored " << scores.blackScore << " points and enclosed " << scores.blackTotalArea << " tiles.\n";
	cout << "White scored " << scores.whiteScore << " points and enclosed " << scores.whiteTotalArea << " tiles.\n";
	if (scores.blackScore > scores.whiteScore)
	{
		cout << "Black wins!\n";
	}
	else if (scores.blackScore < scores.whiteScore)
	{
		cout << "White wins!\n";
	}
	else
	{
		cout << "There was a tie!\n";
	}

	// wait for esc to exit the program
	while (waitKey(10) != 27) {};
	return 0;
}

