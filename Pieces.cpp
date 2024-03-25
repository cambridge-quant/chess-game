// OOP Chess Project: Pieces.cpp.
// This is the Piece class source file.
// It contains all the definitions related to the Piece class.
// James Cummins.

// Include the Pieces header file.
#include "Pieces.h"

// Using namespaces.
using namespace PieceNamespace;

// Default constructor.
Piece::Piece()
{

	// Set all the variables to default values.
	PieceName   = "DefaultName";
	PieceColour = "DefaultColour";
	PiecePoint  = Point();
	PieceSymbol = 'X';
	PieceTurn   = 0;
	LastPiece   = false;

}

// Parameterised constructor.
Piece::Piece(Point InputPoint, string InputName, string InputColour, char InputSymbol)
{

	// Set all the variables to the inputted values.
	PieceName   = InputName;
	PiecePoint  = InputPoint;
	PieceColour = InputColour;
	PieceSymbol = InputSymbol;
	PieceTurn   = 0;
	LastPiece   = false;

}

// Copy constructor.
Piece::Piece(const Piece &piece)
{

	// Copy all the data over.
	PieceName   = piece.PieceName;
	PieceColour = piece.PieceColour;
	PieceSymbol = piece.PieceSymbol;
	PiecePoint  = piece.PiecePoint;
	PieceTurn   = piece.PieceTurn;
	LastPiece   = piece.LastPiece;

}

// Move constructor.
Piece::Piece(Piece &&piece)
{

	// Copy all the data over.
	PieceName   = piece.PieceName;
	PieceColour = piece.PieceColour;
	PieceSymbol = piece.PieceSymbol;
	PiecePoint  = piece.PiecePoint;
	PieceTurn   = piece.PieceTurn;
	LastPiece   = piece.LastPiece;

	// Set the variables of 'piece' to its default values
	piece.PieceName   = "DefaultName";
	piece.PieceColour = "DefaultColour";
	piece.PieceSymbol = 'X';
	piece.PiecePoint  = Point();
	piece.PieceTurn   = 0;
	piece.LastPiece   = false;

}

// Copy assignment operator for deep copying.
Piece & Piece::operator=(const Piece &piece)
{

	// No self assignment.
	if (&piece == this) return *this;
	
	// Copy all the data over.
	PieceName   = piece.PieceName;
	PieceColour = piece.PieceColour;
	PieceSymbol = piece.PieceSymbol;
	PiecePoint  = piece.PiecePoint;
	PieceTurn   = piece.PieceTurn;
	LastPiece   = piece.LastPiece;

	// Special pointer.
	return *this;

}

// Move assignment operator.
Piece & Piece::operator=(Piece &&piece)
{

	// Swap all the data over.
	swap(PieceName,   piece.PieceName);
	swap(PieceColour, piece.PieceColour);
	swap(PieceSymbol, piece.PieceSymbol);
	swap(PiecePoint,  piece.PiecePoint);
	swap(PieceTurn,   piece.PieceTurn);
	swap(LastPiece,   piece.LastPiece);

	// Use the 'this' keyword.
	return *this;

}

// Access functions.
Point  Piece::GetPoint()     const { return PiecePoint; }
string Piece::GetName()      const { return PieceName; }
string Piece::GetColour()    const { return PieceColour; }
char   Piece::GetSymbol()    const { return PieceSymbol; }
int    Piece::GetTurn()      const { return PieceTurn; }
bool   Piece::GetLastPiece() const { return LastPiece; }

// Mutator funcions.
void Piece::SetPosition(Point newPoint)    { PiecePoint = newPoint; }
void Piece::SetTurn(int TurnNumber)        { PieceTurn  = TurnNumber; }
void Piece::IterateTurn(int Iteration)     { PieceTurn += Iteration; }
void Piece::SetLastPiece(bool TrueOrFalse) { LastPiece  = TrueOrFalse; }

// Default constructor.
Pawn::Pawn() : Piece() {}
// Parameterised constructor.
Pawn::Pawn(Point InputPoint, string InputColour, char InputSymbol) : Piece(InputPoint, "Pawn", InputColour, InputSymbol) {}

// Pawn type of move function.
MoveType Pawn::TypeOfMove(int NewX, int NewY) const
{

	// Direction is related to the colour of the piece.
	int Direction;
	// Get the original coordinates of the piece.
	int OldX{ PiecePoint.GetX() };
	int OldY{ PiecePoint.GetY() };

	// If the colour is white, then set the direction as -1.
	// Similarly, if the colour is black, set the direction as +1.
	if (PieceColour == "White") { Direction = -1; }
	if (PieceColour == "Black") { Direction =  1; }
	// If the piece colour is not recognised for some reason...
	// then print an appropriate error message.
	if (PieceColour != "White" && PieceColour != "Black")
	{
		cerr << "Error: Piece colour not recognised." << endl;
	}

	// The horizontal move can't be greater than one.
	if (abs(NewY - OldY) <= 1) {
		// In the case it didn't move horizontally, enter this loop.
		if (abs(NewY - OldY) == 0) {
			// If the vertical move was one space in the correct direction, return PawnMove.
			if (NewX == OldX + Direction) { return PawnMove; }
			// If the vertical mvoe was two spaces forward...
			// and the turn of the piece was zero, return DoubleStep.
			if (NewX == OldX + 2 * Direction && PieceTurn == 0) { return DoubleStep; }
		}
		// If the horizontal move was one space and the verical move was...
		// one space in the correct direction, return PawnCapture.
		if (abs(NewY - OldY) == 1 && NewX == OldX + Direction) { return PawnCapture; }
	}
	// Else if it didn't meet any of the previous requirements, then return false.
	return Illegal;

}

// Pawn position evaluation function.
vector<vector<double>> Pawn::PositionEvaluation() const
{
	// Return the 8x8 pawn position evaluation vector.
	return { {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
	{5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
	{1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
	{0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
	{0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
	{0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
	{0.5,  1.0, 1.0,  -2.0, -2.0,  1.0,  1.0,  0.5},
	{0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0} };
}

// Default constructor.
WhitePawn::WhitePawn() : Pawn() {}
// Parameterised constructor type one.
WhitePawn::WhitePawn(Point InputPoint) : Pawn(InputPoint, "White", 'P') {};
// Parameterised constructor type two.
WhitePawn::WhitePawn(int InputX, int InputY) : Pawn(Point(InputX, InputY), "White", 'P') {};

// WhitePawn get value function.
double WhitePawn::GetValue() const { return  10.0; }

// Default constructor.
BlackPawn::BlackPawn() : Pawn() {}
// Parameterised constructor type one.
BlackPawn::BlackPawn(Point InputPoint) : Pawn(InputPoint, "Black", 'p') {};
// Parameterised constructor type two.
BlackPawn::BlackPawn(int InputX, int InputY) : Pawn(Point(InputX, InputY), "Black", 'p') {};

// BlackPawn get value function.
double BlackPawn::GetValue() const { return -10.0; }

// Default constructor.
King::King() : Piece() {}
// Parameterised constructor.
King::King(Point InputPoint, string InputColour, char InputSymbol) : Piece(InputPoint, "King", InputColour, InputSymbol) {}

// King type of move function.
MoveType King::TypeOfMove(int NewX, int NewY) const
{

	// Get the original coordinates of the piece.
	int OldX{ PiecePoint.GetX() };
	int OldY{ PiecePoint.GetY() };
	// If the move is to a nearest neighbour position, return KingMove.
	if ((abs(NewY - OldY) + abs(NewX - OldX) == 1) || (abs(NewY - OldY) == 1 && abs(NewX - OldX) == 1)) { return KingMove; }
	// Account for castling maneuvers.
	if ( NewX == OldX && OldY == 4 && NewY - OldY ==  2 && (OldX == 0 || OldX == 7) && PieceTurn == 0 ) { return CastleRight; }
	if ( NewX == OldX && OldY == 4 && NewY - OldY == -2 && (OldX == 0 || OldX == 7) && PieceTurn == 0 ) { return CastleLeft; }
	// Else if it didn't meet any of the previous requirements, then return false.
	return Illegal;

}

// King position evaluation function.
vector<vector<double>> King::PositionEvaluation() const
{
	// Return the 8x8 king position evaluation vector.
	return { {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
	{-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
	{2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
	{2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0} };
}

// Default constructor.
WhiteKing::WhiteKing() : King() {}
// Parameterised constructor type one.
WhiteKing::WhiteKing(Point InputPoint) : King(InputPoint, "White", 'K') {};
// Parameterised constructor type two.
WhiteKing::WhiteKing(int InputX, int InputY) : King(Point(InputX, InputY), "White", 'K') {};

// WhiteKing get value function.
double WhiteKing::GetValue() const { return  900.0; }

// Default constructor.
BlackKing::BlackKing() : King() {}
// Parameterised constructor type one.
BlackKing::BlackKing(Point InputPoint) : King(InputPoint, "Black", 'k') {};
// Parameterised constructor type two.
BlackKing::BlackKing(int InputX, int InputY) : King(Point(InputX, InputY), "Black", 'k') {};

// BlackKing get value function.
double BlackKing::GetValue() const { return -900.0; }

// Default constructor.
Queen::Queen() : Piece() {}
// Parameterised constructor.
Queen::Queen(Point InputPoint, string InputColour, char InputSymbol) : Piece(InputPoint, "Queen", InputColour, InputSymbol) {}

// Queen type of move function.
MoveType Queen::TypeOfMove(int NewX, int NewY) const
{

	// Get the original coordinates of the piece.
	int OldX{ PiecePoint.GetX() };
	int OldY{ PiecePoint.GetY() };

	// The move could be along a straight line.
	if ((NewX - OldX < 0) && (NewY == OldY)) { return Up; }
	if ((NewX - OldX > 0) && (NewY == OldY)) { return Down; }
	if ((NewX == OldX) && (NewY - OldY > 0)) { return Right; }
	if ((NewX == OldX) && (NewY - OldY < 0)) { return Left; }

	// Or the move could be along a diagonal.
	if (abs(NewX - OldX) == abs(NewY - OldY)) {
		if ((NewX - OldX < 0) && (NewY - OldY > 0)) { return DiagUR; }
		if ((NewX - OldX > 0) && (NewY - OldY < 0)) { return DiagDL; }
		if ((NewX - OldX > 0) && (NewY - OldY > 0)) { return DiagDR; }
		if ((NewX - OldX < 0) && (NewY - OldY < 0)) { return DiagUL; }
	}
	// Or it could be neither, in which case it would be an illegal move.
	return Illegal;

}

// Queen position evaluation function.
vector<vector<double>> Queen::PositionEvaluation() const
{
	// Return the 8x8 queen position evaluation vector.
	return { {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
	{-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
	{-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
	{-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
	{0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
	{-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
	{-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
	{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0} };
}

// Default constructor.
WhiteQueen::WhiteQueen() : Queen() {}
// Parameterised constructor type one.
WhiteQueen::WhiteQueen(Point InputPoint) : Queen(InputPoint, "White", 'Q') {};
// Parameterised constructor type two.
WhiteQueen::WhiteQueen(int InputX, int InputY) : Queen(Point(InputX, InputY), "White", 'Q') {};

// WhiteQueen get value function.
double WhiteQueen::GetValue() const { return  90.0; }

// Default constructor.
BlackQueen::BlackQueen() : Queen() {}
// Parameterised constructor type one.
BlackQueen::BlackQueen(Point InputPoint) : Queen(InputPoint, "Black", 'q') {};
// Parameterised constructor type two.
BlackQueen::BlackQueen(int InputX, int InputY) : Queen(Point(InputX, InputY), "Black", 'q') {};

// BlackQueen get value function.
double BlackQueen::GetValue() const { return -90.0; }

// Default constructor.
Rook::Rook() : Piece() {}
// Parameterised constructor.
Rook::Rook(Point InputPoint, string InputColour, char InputSymbol) : Piece(InputPoint, "Rook", InputColour, InputSymbol) {}

// Rook type of move function.
MoveType Rook::TypeOfMove(int NewX, int NewY) const
{

	// Get the original coordinates of the piece.
	int OldX{ PiecePoint.GetX() };
	int OldY{ PiecePoint.GetY() };

	// Rooks can only move along straight lines and not diagonals.
	if ((NewX - OldX < 0) && (NewY == OldY)) { return Up; }
	if ((NewX - OldX > 0) && (NewY == OldY)) { return Down; }
	if ((NewX == OldX) && (NewY - OldY > 0)) { return Right; }
	if ((NewX == OldX) && (NewY - OldY < 0)) { return Left; }
	// If this isn't the case, then return illegal.
	return Illegal;

}

// Rook position evaluation function.
vector<vector<double>> Rook::PositionEvaluation() const
{
	// Return the 8x8 rook position evaluation vector.
	return { {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
	{0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{0.0,   0.0, 0.0,  0.5,  0.5,  0.0,  0.0,  0.0} };
}

// Default constructor.
WhiteRook::WhiteRook() : Rook() {}
// Parameterised constructor type one.
WhiteRook::WhiteRook(Point InputPoint) : Rook(InputPoint, "White", 'R') {};
// Parameterised constructor type two.
WhiteRook::WhiteRook(int InputX, int InputY) : Rook(Point(InputX, InputY), "White", 'R') {};

// WhiteRook get value function.
double WhiteRook::GetValue() const { return  50.0; }

// Default constructor.
BlackRook::BlackRook() : Rook() {}
// Parameterised constructor type one.
BlackRook::BlackRook(Point InputPoint) : Rook(InputPoint, "Black", 'r') {};
// Parameterised constructor type two.
BlackRook::BlackRook(int InputX, int InputY) : Rook(Point(InputX, InputY), "Black", 'r') {};

// BlackRook get value function.
double BlackRook::GetValue() const { return -50.0; }

// Default constructor.
Bishop::Bishop() : Piece() {}
// Parameterised constructor.
Bishop::Bishop(Point InputPoint, string InputColour, char InputSymbol) : Piece(InputPoint, "Bishop", InputColour, InputSymbol) {}

// Bishop type of move function.
MoveType Bishop::TypeOfMove(int NewX, int NewY) const
{

	// Get the original coordinates of the piece.
	int OldX{ PiecePoint.GetX() };
	int OldY{ PiecePoint.GetY() };

	// If the move is not a diagonal return illegal.
	if ( abs(NewX - OldX) != abs(NewY - OldY) ) { return Illegal; }
	// If it is a diagonal, the return the appropriate value.
	if ((NewX - OldX < 0) && (NewY - OldY > 0)) { return DiagUR; }
	if ((NewX - OldX > 0) && (NewY - OldY < 0)) { return DiagDL; }
	if ((NewX - OldX > 0) && (NewY - OldY > 0)) { return DiagDR; }
	if ((NewX - OldX < 0) && (NewY - OldY < 0)) { return DiagUL; }
	// If for some reason none of the these conditions were met, return illegal.
	return Illegal;

}

// Bishop position evaluation function.
vector<vector<double>> Bishop::PositionEvaluation() const
{
	// Return the 8x8 bishop position evaluation vector.
	return { {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 },
	{-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
	{-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
	{-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
	{-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
	{-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
	{-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
	{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0} };
}

// Default constructor.
WhiteBishop::WhiteBishop() : Bishop() {}
// Parameterised constructor type one.
WhiteBishop::WhiteBishop(Point InputPoint) : Bishop(InputPoint, "White", 'B') {};
// Parameterised constructor type two.
WhiteBishop::WhiteBishop(int InputX, int InputY) : Bishop(Point(InputX, InputY), "White", 'B') {};

// WhiteBishop get value function.
double WhiteBishop::GetValue() const { return  30.0; }

// Default constructor.
BlackBishop::BlackBishop() : Bishop() {}
// Parameterised constructor type one.
BlackBishop::BlackBishop(Point InputPoint) : Bishop(InputPoint, "Black", 'b') {};
// Parameterised constructor type two.
BlackBishop::BlackBishop(int InputX, int InputY) : Bishop(Point(InputX, InputY), "Black", 'b') {};

// BlackBishop get value function.
double BlackBishop::GetValue() const { return -30.0; }

// Default constructor.
Knight::Knight() : Piece() {}
// Parameterised constructor.
Knight::Knight(Point InputPoint, string InputColour, char InputSymbol) : Piece(InputPoint, "Knight", InputColour, InputSymbol) {}

// Knight type of move function.
MoveType Knight::TypeOfMove(int NewX, int NewY) const
{

	// Get the original coordinates of the piece.
	int OldX{ PiecePoint.GetX() };
	int OldY{ PiecePoint.GetY() };

	// Possible moves are 'L' moves.
	if ((abs(NewX - OldX) + abs(NewY - OldY) == 3) && (abs(NewX - OldX) <= 2) && (abs(NewY - OldY) <= 2))
	{
		return KnightMove;
	}
	// If it's not an 'L' move then return illegal.
	return Illegal;

}

// King position evaluation function.
vector<vector<double>> Knight::PositionEvaluation() const
{
	// Return the 8x8 knight position evaluation vector.
	return { {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
	{-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
	{-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
	{-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
	{-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
	{-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
	{-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
	{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0} };
}

// Default constructor.
WhiteKnight::WhiteKnight() : Knight() {}
// Parameterised constructor type one.
WhiteKnight::WhiteKnight(Point InputPoint) : Knight(InputPoint, "White", 'N') {};
// Parameterised constructor type two.
WhiteKnight::WhiteKnight(int InputX, int InputY) : Knight(Point(InputX, InputY), "White", 'N') {};

// WhiteKnight get value function.
double WhiteKnight::GetValue() const { return  30.0; }

// Default constructor.
BlackKnight::BlackKnight() : Knight() {}
// Parameterised constructor type one.
BlackKnight::BlackKnight(Point InputPoint) : Knight(InputPoint, "Black", 'n') {};
// Parameterised constructor type two.
BlackKnight::BlackKnight(int InputX, int InputY) : Knight(Point(InputX, InputY), "Black", 'n') {};

// BlackKnight get value function.
double BlackKnight::GetValue() const { return -30.0; }