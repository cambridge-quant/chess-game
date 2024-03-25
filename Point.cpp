// OOP Chess Project: Point.cpp.
// This is the Point class source file.
// It contains all the definitions related to the Point class.
// James Cummins.

// Include the Point header file.
#include "Point.h"

// Using namespaces.
using namespace PointNamespace;
using namespace std;

// Default constructor.
Point::Point()
{

	// Set the coordinates equal to zero.
	xCoordinate = 0;
	yCoordinate = 0;

}

// Parameterised constructor.
Point::Point(int InputX, int InputY)
{

	// Check if the input is good.
	if (InputX >= 0 && InputX < 8 && InputY >= 0 && InputY < 8)
	{
		// Set the member data equal to the inputted data.
		xCoordinate = InputX;
		yCoordinate = InputY;
	}
	// If the input is not good, print an error message.
	else
	{
		cerr << "Error: Point parameterised constructor failed." << endl;
	}

}

// Access functions to return the coordinates.
// The 'const' keyword is made use of.
int Point::GetX() const { return xCoordinate; }
int Point::GetY() const { return yCoordinate; }

// Mutator function to set the coordinates of a point.
void Point::SetPoint(int InputX, int InputY)
{

	// Check if the input is good.
	if (InputX >= 0 && InputX < 8 && InputY >= 0 && InputY < 8)
	{
		// Set the member data equal to the inputted data.
		xCoordinate = InputX;
		yCoordinate = InputY;
	}
	// If the input is not good, print an error message.
	else
	{
		cerr << "Error: Could not set the point position." << endl;
	}

}