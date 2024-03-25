// OOP Chess Project: Pieces.h.
// This is the Piece class header file.
// It contains all the declarations related to the Piece class.
// James Cummins.

#pragma once
// This will be true only once.
#ifndef MY_CLASS_Pieces
#define MY_CLASS_Pieces

// Include the relevant libraries.
#include <string>
#include <vector>
#include "Point.h"

// Using namespaces.
using namespace PointNamespace;
using namespace std;

// Using a namespace to avoid name collisions.
namespace PieceNamespace
{

	// All the possible move types
	enum MoveType {
		PawnMove,     // Relevant for pawns.
		PawnCapture,  //         "
		DoubleStep,   //         "
		KingMove,     // Relevant for kings.
		CastleRight,  //         "
		CastleLeft,   //         "
		KnightMove,   // Relevant for knights.
		Left,         // Relevant for rooks and queens.
		Right,        //         "
		Up,           //         "
		Down,         //         "
		DiagUR,       // Relevant for bishops and queens.
		DiagUL,       //         "
		DiagDL,       //         "
		DiagDR,       //         "
		Illegal       // Applies to all pieces.
	};

	// Virtual Piece class.
	class Piece {

	// Protected member data.
	protected:

		// The defining features of a piece.
		string PieceName;
		string PieceColour;
		char   PieceSymbol;
		Point  PiecePoint;
		int    PieceTurn;
		bool   LastPiece;

	// Public member functions.
	public:

		// Default constructor.
		Piece();
		// Parameterised constructor.
		Piece(Point InputPoint, string InputName, string InputColour, char InputSymbol);
		// Virtual destructor.
		virtual ~Piece() {}

		// Move semantics:
		// Copy constructor.
		Piece(const Piece &piece);
		// Move constructor.
		Piece(Piece &&piece);
		// Copy assignment operator for deep copying.
		Piece & operator=(const Piece &piece);
		// Move assignment operator.
		Piece & operator=(Piece &&piece);

		// Access functions.
		Point  GetPoint()     const;
		string GetName()      const;
		string GetColour()    const;
		char   GetSymbol()    const;
		int    GetTurn()      const;
		bool   GetLastPiece() const;

		// Mutator functions.
		void SetPosition(Point newPoint);
		void SetTurn(int TurnNumber);
		void IterateTurn(int Iteration);
		void SetLastPiece(bool TrueOrFalse);

		// Virtual functions.
		virtual MoveType TypeOfMove(int, int) const = 0;
		virtual double GetValue() const = 0;
		virtual vector<vector<double>> PositionEvaluation() const = 0;

	};

	// Derived Pawn class.
	class Pawn : public Piece {

	// Public member functions.
	public:

		// Default constructor.
		Pawn();
		// Parameterised constructor.
		Pawn(Point InputPoint, string InputColour, char InputSymbol);
		// Destructor
		~Pawn() {}

		// Overriden virtual functions.
		MoveType TypeOfMove(int newX, int newY) const;
		vector<vector<double>> PositionEvaluation() const;

	};

	// Derived WhitePawn class.
	class WhitePawn : public Pawn {

	// Public member functions.
	public:

		// Default constructor.
		WhitePawn();
		// Parameterised constructor type one.
		WhitePawn(Point InputPoint);
		// Parameterised constructor type two.
		WhitePawn(int InputX, int InputY);
		// Destructor.
		~WhitePawn() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived BlackPawn class.
	class BlackPawn : public Pawn {

	// Public member functions.
	public:

		// Default constructor.
		BlackPawn();
		// Parameterised constructor type one.
		BlackPawn(Point InputPoint);
		// Parameterised constructor type two.
		BlackPawn(int InputX, int InputY);
		// Destructor.
		~BlackPawn() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived King class.
	class King : public Piece {

	// Public member functions.
	public:

		// Default constructor.
		King();
		// Parameterised constructor.
		King(Point InputPoint, string InputColour, char InputSymbol);
		// Destructor.
		~King() {}

		// Overriden virtual functions.
		MoveType TypeOfMove(int newX, int newY) const;
		vector<vector<double>> PositionEvaluation() const;

	};

	// Derived WhiteKing class.
	class WhiteKing : public King {

	// Public member functions.
	public:

		// Default constructor.
		WhiteKing();
		// Parameterised constructor type one.
		WhiteKing(Point InputPoint);
		// Parameterised constructor type two.
		WhiteKing(int InputX, int InputY);
		// Destructor.
		~WhiteKing() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived BlackKing class.
	class BlackKing : public King {

	// Public member functions.
	public:

		// Default constructor.
		BlackKing();
		// Parameterised constructor type one.
		BlackKing(Point InputPoint);
		// Parameterised constructor type two.
		BlackKing(int InputX, int InputY);
		// Destructor.
		~BlackKing() {}

		// Overriden member function.
		double GetValue() const;

	};

	// Derived Queen class.
	class Queen : public Piece {

	// Public member functions.
	public:

		// Default constructor.
		Queen();
		// Parameterised constructor.
		Queen(Point InputPoint, string InputColour, char InputSymbol);
		// Destructor.
		~Queen() {}

		// Overriden virtual functions.
		MoveType TypeOfMove(int newX, int newY) const;
		vector<vector<double>> PositionEvaluation() const;

	};

	// Derived WhiteQueen class.
	class WhiteQueen : public Queen {

	// Public member functions.
	public:

		// Default constructor.
		WhiteQueen();
		// Parameterised constructor type one.
		WhiteQueen(Point InputPoint);
		// Parameterised constructor type two.
		WhiteQueen(int InputX, int InputY);
		// Destructor.
		~WhiteQueen() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived BlackQueen class.
	class BlackQueen : public Queen {

	// Public member functions.
	public:

		// Default constructor.
		BlackQueen();
		// Parameterised constructor type one.
		BlackQueen(Point InputPoint);
		// Parameterised constructor type two.
		BlackQueen(int InputX, int InputY);
		// Destructor.
		~BlackQueen() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived Rook class.
	class Rook : public Piece {

	// Public member functions.
	public:

		// Default constructor.
		Rook();
		// Parameterised constructor.
		Rook(Point InputPoint, string InputColour, char InputSymbol);
		// Destructor.
		~Rook() {}

		// Overriden virtual functions.
		MoveType TypeOfMove(int newX, int newY) const;
		vector<vector<double>> PositionEvaluation() const;

	};

	// Derived WhiteRook class.
	class WhiteRook : public Rook {

	// Public member functions.
	public:

		// Default constructor.
		WhiteRook();
		// Parameterised constructor type one.
		WhiteRook(Point InputPoint);
		// Parameterised constructor type two.
		WhiteRook(int InputX, int InputY);
		// Destructor.
		~WhiteRook() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived BlackRook class.
	class BlackRook : public Rook {

	// Public member functions.
	public:

		// Default constructor.
		BlackRook();
		// Parameterised constructor type one.
		BlackRook(Point InputPoint);
		// Parameterised constructor type two.
		BlackRook(int InputX, int InputY);
		// Destructor.
		~BlackRook() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived Bishop class.
	class Bishop : public Piece {

	// Public member functions.
	public:

		// Default constructor.
		Bishop();
		// Parameterised constructor.
		Bishop(Point InputPoint, string InputColour, char InputSymbol);
		// Destructor.
		~Bishop() {}

		// Overriden virtual functions.
		MoveType TypeOfMove(int newX, int newY) const;
		vector<vector<double>> PositionEvaluation() const;

	};

	// Derived WhiteBishop class.
	class WhiteBishop : public Bishop {

	// Public member functions.
	public:

		// Default constructor.
		WhiteBishop();
		// Parameterised constructor type one.
		WhiteBishop(Point InputPoint);
		// Parameterised constructor type two.
		WhiteBishop(int InputX, int InputY);
		// Destructor.
		~WhiteBishop() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived BlackBishop class.
	class BlackBishop : public Bishop {

	// Public member functions.
	public:

		// Default constructor.
		BlackBishop();
		// Parameterised constructor type one.
		BlackBishop(Point InputPoint);
		// Parameterised constructor type two.
		BlackBishop(int InputX, int InputY);
		// Destructor.
		~BlackBishop() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived Knight class.
	class Knight : public Piece {

	// Public member functions.
	public:

		// Default constructor.
		Knight();
		// Parameterised constructor.
		Knight(Point InputPoint, string InputColour, char InputSymbol);
		// Destructor.
		~Knight() {}

		// Overriden virtual functions.
		MoveType TypeOfMove(int newX, int newY) const;
		vector<vector<double>> PositionEvaluation() const;

	};

	// Derived WhiteKnight class.
	class WhiteKnight : public Knight {

	// Public member functions.
	public:

		// Default constructor.
		WhiteKnight();
		// Parameterised constructor type one.
		WhiteKnight(Point InputPoint);
		// Parameterised constructor type two.
		WhiteKnight(int InputX, int InputY);
		// Destructor.
		~WhiteKnight() {}

		// Overriden virtual function.
		double GetValue() const;

	};

	// Derived BlackKnight class.
	class BlackKnight : public Knight {

	// Public member functions.
	public:

		// Default constructor.
		BlackKnight();
		// Parameterised constructor type one.
		BlackKnight(Point InputPoint);
		// Parameterised constructor type two.
		BlackKnight(int InputX, int InputY);
		// Destructor.
		~BlackKnight() {}

		// Overriden virtual function.
		double GetValue() const;

	};

}

#endif