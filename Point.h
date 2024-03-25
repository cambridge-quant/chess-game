// OOP Chess Project: Point.h.
// This is the Point class header file.
// It contains all the declarations related to the Point class.
// James Cummins.

#pragma once
// This will only be true once.
#ifndef MY_CLASS_Point
#define MY_CLASS_Point

// Include the iostream library.
#include <iostream>

// Using a namespace to avoid name collisions.
namespace PointNamespace
{

	// Point class.
	class Point {

	// Private member data.
	private:

		// Each point on the chessboard is defined...
		// by its x coordinate and y coordinate.
		int xCoordinate;
		int yCoordinate;

	// Public member functions.
	public:

		// Default constructor.
		Point();
		// Parameterised constructor.
		Point(int InputX, int InputY);
		// Destructor.
		~Point() {}

		// Access functions.
		int GetX() const;
		int GetY() const;

		// Mutator function.
		void SetPoint(int InputX, int InputY);

	};

}

#endif