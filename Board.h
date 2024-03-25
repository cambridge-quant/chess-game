// OOP Chess Project: Board.h.
// This is the Board class header file.
// It contains all the declarations related to the Board class.
// James Cummins.

#pragma once
// This will be true only one.
#ifndef MY_CLASS_Board
#define MY_CLASS_Board

// Include the relevant libraries.
#include <windows.h>
#include <exception>
#include <memory>
#include "Pieces.h"

// Using namespaces.
using namespace PieceNamespace;
using namespace std;

// Using a namespace to avoid name collisions.
namespace BoardNamespace
{

	// Board class.
	class Board {

	// Private member data.
	private:

		// A 2D vector of Piece shared pointers representing the chessboard.
		vector<vector<shared_ptr<Piece>>> ChessBoard;

		// The coordinates of the moved computer pieces.
		int ComputerOriginalX, ComputerOriginalY, ComputerMovedX, ComputerMovedY;
		// The pieces that the computer moves.
		shared_ptr<Piece> ComputerMovedPiece, ComputerCapturedPiece, ComputerEnPassantPiece, ComputerPromotedPawn;

	// Public member functions.
	public:

		// Default constructor.
		Board();
		// Destructor.
		~Board() {}

		// Access functions.
		char GetBoardSymbol(int xCoordinate, int yCoordinate) const;
		shared_ptr<Piece> GetPiece(Point InputPoint)          const;
		shared_ptr<Piece> GetPiece(int InputX, int InputY)    const;

		// More access functions.
		int GetComputerOriginalX() const;
		int GetComputerOriginalY() const;
		int GetComputerMovedX()    const;
		int GetComputerMovedY()    const;
		shared_ptr<Piece> GetComputerMovedPiece()     const;
		shared_ptr<Piece> GetComputerCapturedPiece()  const;
		shared_ptr<Piece> GetComputerEnPassantPiece() const;
		shared_ptr<Piece> GetComputerPromotedPawn()   const;

		// Mutator functions.
		void SetComputerPieces(shared_ptr<Piece> MovedPiece, shared_ptr<Piece> CapturedPiece);
		void SetComputerPositions(int OldX, int OldY, int NewX, int NewY);
		void SetComputerEnPassantPiece(shared_ptr<Piece> EnPassantPiece);
		void SetComputerPromotedPawn(shared_ptr<Piece> PromotedPawnPiece);

		// Function to intialise the chessboard.
		void InitialiseBoard();

		// Function to set the colour of the writing and the background.
		void SetColourAndBackground(int ForegroundColour, int BackgroundColour);

		// Function to display the chessboard.
		void PrintBoard();

		// Function to move a piece and update the relevant values.
		void MovePiece(int OldX, int OldY, int NewX, int NewY);

		// Function to move a piece and do nothing else.
		void MovePieceAndDoNothing(int OldX, int OldY, int NewX, int NewY);

		// Function to check if the move is an en passant move.
		bool EnPassantMove(int OldX, int OldY, int NewX, int NewY);

		// Function to check if the move is a pawn promotion move.
		bool PawnPromotionMove(int OldX, int OldY, int NewX, int NewY);

		// Function to check if the move is a castling move.
		bool CastlingMove(int OldX, int OldY, int NewX, int NewY);

		// Function to check if the piece at (OldX, OldY) can move to (NewX, NewY).
		bool CanPieceMove(int OldX, int OldY, int NewX, int NewY);
			
		// Function to promote a pawn at (xCoordinate, yCoordinate).
		void PromotePawn(int xCoordinate, int yCoordinate);

		// Function to unpromote a pawn at (xCoordinate, yCoordinate).
		void UnPromotePawn(int xCoordinate, int yCoordinate);

		// Function to check if the king is in check.
		bool KingInCheck(string Colour);

		// Function to set the variable 'LastPiece' to false for every piece.
		void SetBoardLastPieceFalse();

		// Function to return a quantifiable value of the strength of the chessboard.
		double EvaluateBoard();

		// Function to add a piece onto the chessboard.
		void AddPiece(shared_ptr<Piece> ChessPiece);

	};

}

#endif