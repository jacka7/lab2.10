#ifdef TRIANGULATION_EXPORTS
#define TRIANGULATIONLIBRARY_API __declspec(dllexport)
#else
#define TRIANGULATION_API __declspec(dllimport)
#endif 

 
// Library
#include "stdafx.h"


#ifndef TRIANGULATION_H
#define TRIANGULATION_H


//	TYPES

#include <windows.h>

// LINE Structure
// Represents line between two points
struct LINE
{
	COORD From, To;
	ULONG64 Length;
};

// List of points
typedef std::vector<COORD> PointList;

// List of lines
typedef std::vector<LINE> LineList;


//	FUNCTIONS

#ifndef TYPE_INCLUDE_ONLY

//	generates trinagulation from input point list
//	using greedy triangulation algorithm
TRIANGULATION_API LineList Triangulate(const PointList&);

#endif

#endif