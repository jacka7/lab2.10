// triangulation.cpp :

#include "stdafx.h"
#include "triangulation.h"
 

using namespace std;

//	Inputs PointCount points from console
//	Testing Function
PointList InputPoints(size_t PointCount)
{
	PointList Points;
	for (size_t i = 0; i < PointCount; i++)
	{
		COORD currPoint;
		std::cin >> currPoint.X >> currPoint.Y;
		Points.push_back(currPoint);
	}

	return Points;
}


//	Prints list of points to console
//	Testing function
void PrintPoints(const PointList& Points)
{
	for (size_t i = 0; i < Points.size(); i++)
		std::cout << Points[i].X << " " << Points[i].Y << "\n";
}


// Generate all lines between all points from list
LineList BuildLines(const PointList& Points)
{
	size_t PointCount = Points.size();
	LineList Lines;
	
	for (size_t i = 0; i < PointCount; i++)
	for (size_t j = i + 1; j < PointCount; j++)
	{
		LINE currLine;
		currLine.From = Points[i];
		currLine.To = Points[j];
		currLine.Length =
			(currLine.From.X - currLine.To.X) * (currLine.From.X - currLine.To.X) +
			(currLine.From.Y - currLine.To.Y) * (currLine.From.Y - currLine.To.Y);
		Lines.push_back(currLine);
	}

	return Lines;
}


//	Prints list of lines to console
//	Testing function
void PrintLines(LineList Lines)
{
	for (size_t i = 0; i < Lines.size(); i++)
		std::cout << Lines[i].From.X << " "
		<< Lines[i].From.Y << " - "
		<< Lines[i].To.X << " "
		<< Lines[i].To.Y << "\n";
}


// Runs bubble sort of lines list
void BubbleSort(LineList& Lines)
{
	for (size_t i = 0; i < Lines.size() - 1; i++)
	for (size_t j = i + 1; j < Lines.size(); j++)
	if (Lines[i].Length > Lines[j].Length)
	{
		LINE Temp	= Lines[i];
		Lines[i]	= Lines[j];
		Lines[j]	= Temp;
	}
}


// Returns true if two lines is intersect
bool Intersection(LINE Line1, LINE Line2)
{


	INT64 TempCalc1	= (Line2.To.X - Line2.From.X) * (Line1.From.Y	- Line2.From.Y)
					- (Line2.To.Y - Line2.From.Y) * (Line1.From.X	- Line2.From.X);
	INT64 TempCalc2	= (Line2.To.X - Line2.From.X) * (Line1.To.Y - Line2.From.Y)
					- (Line2.To.Y - Line2.From.Y) * (Line1.To.X	- Line2.From.X);
	INT64 TempCalc3	= (Line1.To.X - Line1.From.X) * (Line2.From.Y - Line1.From.Y)
					- (Line1.To.Y - Line1.From.Y) * (Line2.From.X	- Line1.From.X);
	INT64 TempCalc4	= (Line1.To.X - Line1.From.X) * (Line2.To.Y - Line1.From.Y)
					- (Line1.To.Y - Line1.From.Y) * (Line2.To.X	- Line1.From.X);
	return (TempCalc1 * TempCalc2 < 0)
		&& (TempCalc3 * TempCalc4 < 0);
}

LineList Triangulate(const PointList& Points)
{
	//	result
	LineList Result;

	//	if we can triangulate
	if (Points.size() >= 2)
	{
		//	generation and sort all lines 
		LineList Lines = BuildLines(Points);
		BubbleSort(Lines);

		Result.push_back(Lines[0]);
		for (size_t i = 1; i < Lines.size(); i++)
		{
			//	finding intersection with lines in triangulation (result)
			bool IntersectFound = false;
			for (size_t j = 0; j < Result.size(); j++)
				if (Intersection(Lines[i], Result[j]))
					IntersectFound = true;

			//	add to result if haven't found intersection
			if (!IntersectFound)
				Result.push_back(Lines[i]);
		}
	}

	//	returning result
	return Result;
}
