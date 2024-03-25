// OOP Chess Project: Board.cpp.
// This is the Board class source file.
// It contains all the definitions related to the Board class.
// James Cummins.

// Include the Board header file.
#include "Board.h"

// Using namespaces.
using namespace BoardNamespace;

// Non member handle function. This is important for the colour function.
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Non member rotate function. This is important for rotating the position evaluation 2D vector.
void Rotate(vector<vector<double>>& Vector)
{

	// Find the dimensions of the 2D vector (will be 8x8).
	size_t M = Vector.size();
	size_t N = Vector[0].size();

	// Iterate over the vector, swapping values as appropriate.
	for (size_t i = 0; i < M / 2; i++) {
		for (size_t j = 0; j < N; j++) {
			swap(Vector[i][j], Vector[M - i - 1][N - j - 1]);
		}
	}
	if (M & 1) {
		for (size_t i = 0; i < N / 2; i++) {
			swap(Vector[M / 2][i], Vector[M / 2][N - i - 1]);
		}
	}

}

// Default constructor.
Board::Board()
{

	// Exception handling
	try
	{

		// Resize the 2D vector to one that is 8x8
		ChessBoard.resize(8);
		for (auto& column : ChessBoard) { column.resize(8); }
		
		// Fill the chessboard full of null pointers.
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				ChessBoard[i][j] = nullptr;
			}
		}

	}
	// If the code comes here, print an appropriate error message.
	catch (exception& e)
	{
		cerr << "Error: " << e.what() << endl;
	}

	// Set the computer coordinates to zero.
	ComputerOriginalX = ComputerOriginalY = ComputerMovedX = ComputerMovedY = 0;
	// Set the computer pieces to null pointers.
	ComputerMovedPiece = ComputerCapturedPiece = ComputerEnPassantPiece = ComputerPromotedPawn = nullptr;

}

// Access functions.
char Board::GetBoardSymbol(int xCoordinate, int yCoordinate) const { return ChessBoard[xCoordinate][yCoordinate]->GetSymbol(); }
// Overloading the GetPiece function.
shared_ptr<Piece> Board::GetPiece(Point InputPoint)          const { return ChessBoard[InputPoint.GetX()][InputPoint.GetY()]; }
shared_ptr<Piece> Board::GetPiece(int InputX, int InputY)    const { return ChessBoard[InputX][InputY]; }

// More access functions.
int Board::GetComputerOriginalX() const { return ComputerOriginalX; }
int Board::GetComputerOriginalY() const { return ComputerOriginalY; }
int Board::GetComputerMovedX()    const { return ComputerMovedX; }
int Board::GetComputerMovedY()    const { return ComputerMovedY; }
shared_ptr<Piece> Board::GetComputerMovedPiece()     const { return ComputerMovedPiece; }
shared_ptr<Piece> Board::GetComputerCapturedPiece()  const { return ComputerCapturedPiece; }
shared_ptr<Piece> Board::GetComputerEnPassantPiece() const { return ComputerEnPassantPiece; }
shared_ptr<Piece> Board::GetComputerPromotedPawn()   const { return ComputerPromotedPawn; }

// Mutator function to set the moved and captured computer pieces.
void Board::SetComputerPieces(shared_ptr<Piece> MovedPiece, shared_ptr<Piece> CapturedPiece)
{
	ComputerMovedPiece = MovedPiece;
	ComputerCapturedPiece = CapturedPiece;
}

// Mutator function to set the computer coordinates.
void Board::SetComputerPositions(int OldX, int OldY, int NewX, int NewY)
{
	ComputerOriginalX = OldX;
	ComputerOriginalY = OldY;
	ComputerMovedX = NewX;
	ComputerMovedY = NewY;
}

// Mutator functions to set the enpassant and promoted pawn computer pieces.
void Board::SetComputerEnPassantPiece(shared_ptr<Piece> EnPassantPiece)  { ComputerEnPassantPiece = EnPassantPiece; }
void Board::SetComputerPromotedPawn(shared_ptr<Piece> PromotedPawnPiece) { ComputerPromotedPawn = PromotedPawnPiece; }

// Function to initialise the board
void Board::InitialiseBoard()
{

	// Set all the pawns.
	for (int i = 0; i < 8; i++)
	{
		ChessBoard[1][i] = make_shared<BlackPawn>(1, i);
		ChessBoard[6][i] = make_shared<WhitePawn>(6, i);
	}
	
	// Set all the other pieces.
	ChessBoard[0][4] = make_shared<BlackKing>(0, 4);
	ChessBoard[7][4] = make_shared<WhiteKing>(7, 4);
	ChessBoard[0][3] = make_shared<BlackQueen>(0, 3);
	ChessBoard[7][3] = make_shared<WhiteQueen>(7, 3);
	ChessBoard[0][0] = make_shared<BlackRook>(0, 0);
	ChessBoard[0][7] = make_shared<BlackRook>(0, 7);
	ChessBoard[7][0] = make_shared<WhiteRook>(7, 0);
	ChessBoard[7][7] = make_shared<WhiteRook>(7, 7);
	ChessBoard[0][2] = make_shared<BlackBishop>(0, 2);
	ChessBoard[0][5] = make_shared<BlackBishop>(0, 5);
	ChessBoard[7][2] = make_shared<WhiteBishop>(7, 2);
	ChessBoard[7][5] = make_shared<WhiteBishop>(7, 5);
	ChessBoard[0][1] = make_shared<BlackKnight>(0, 1);
	ChessBoard[0][6] = make_shared<BlackKnight>(0, 6);
	ChessBoard[7][1] = make_shared<WhiteKnight>(7, 1);
	ChessBoard[7][6] = make_shared<WhiteKnight>(7, 6);

	// Set all the empty spaces as null pointers.
	// This is important when undoing a move.
	for (int i = 2; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ChessBoard[i][j] = nullptr;
		}
	}

}

// This function allows the colour of the text and background to be set with integers.
void Board::SetColourAndBackground(int ForegroundColour, int BackgroundColour)
{
	WORD WordColour = ((BackgroundColour & 0x0F) << 4) + (ForegroundColour & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WordColour);
}

// Function to print the board.
void Board::PrintBoard()
{

	// Print the coordinate labels for the user.
	cout << "\n        a     b     c     d     e     f     g     h   " << endl;
	cout << "      _____ _____ _____ _____ _____ _____ _____ _____ ";
	for (int i = 0; i < 8; i++) {
		cout << "\n     |     |     |     |     |     |     |     |     |";
		cout << "\n  " << 8 - i << "  ";
		for (int j = 0; j < 8; j++)
		{
			cout << "|  ";
			// If the chessboard is not empty at (i, j), enter this section.
			if (ChessBoard[i][j] != nullptr)
			{
				// If the piece is white, then set the colour and backgound
				if (ChessBoard[i][j]->GetColour() == "White") { SetColourAndBackground(0, 15); }
				// Print the symbol of the piece.
				cout << ChessBoard[i][j]->GetSymbol();
				// Reset the colour and background.
				SetColourAndBackground(15, 0);
			}
			else { cout << " "; }
			cout << "  ";
		}
		cout << "|  " << 8 - i << "\n     |_____|_____|_____|_____|_____|_____|_____|_____|";
	}
	cout << "\n\n        a     b     c     d     e     f     g     h   " << endl;

}

// Function to move a piece and update the relevant values.
void Board::MovePiece(int OldX, int OldY, int NewX, int NewY)
{

	// Create a temporary piece that is the piece to be moved.
	shared_ptr<Piece> TemporaryPiece{ ChessBoard[OldX][OldY] };
	bool PromotePawnBool{ false };

	// Set the relevant computer coordinates and pieces.
	SetComputerPositions(OldX, OldY, NewX, NewY);
	SetComputerPieces(ChessBoard[OldX][OldY], ChessBoard[NewX][NewY]);

	// In the case that the move is/is not an en passant move, set the appropriate pieces.
	if (!EnPassantMove(OldX, OldY, NewX, NewY)) { ComputerEnPassantPiece = nullptr; }
	if (EnPassantMove(OldX, OldY, NewX, NewY) && NewX == 2)
	{
		ComputerEnPassantPiece = ChessBoard[3][NewY];
		ChessBoard[3][NewY] = nullptr;
	}
	if (EnPassantMove(OldX, OldY, NewX, NewY) && NewX == 5)
	{
		ComputerEnPassantPiece = ChessBoard[4][NewY];
		ChessBoard[4][NewY] = nullptr;
	}

	// In the case that the move is a castling move, set the appropriate pieces.
	if (CastlingMove(OldX, OldY, NewX, NewY) && NewY - OldY == 2)
	{
		shared_ptr<Piece> TemporaryCastledPiece{ ChessBoard[OldX][NewY + 1] };
		TemporaryCastledPiece->SetPosition(Point(OldX, NewY - 1));
		ChessBoard[OldX][NewY - 1] = TemporaryCastledPiece;
		ChessBoard[OldX][NewY + 1] = nullptr;
	}
	if (CastlingMove(OldX, OldY, NewX, NewY) && NewY - OldY == -2)
	{
		shared_ptr<Piece> TemporaryCastledPiece = ChessBoard[OldX][NewY - 2];
		TemporaryCastledPiece->SetPosition(Point(OldX, NewY + 1));
		ChessBoard[OldX][NewY + 1] = TemporaryCastledPiece;
		ChessBoard[OldX][NewY - 2] = nullptr;
	}

	// In the case that the move is a pawn promotion move.
	if (PawnPromotionMove(OldX, OldY, NewX, NewY))
	{
		ComputerPromotedPawn = ChessBoard[OldX][OldY];
		PromotePawnBool = true;
	}
	else { ComputerPromotedPawn = nullptr; }

	// Set the new position of the piece.
	TemporaryPiece->SetPosition(Point(NewX, NewY));
	// Iterate the turn number of the piece.
	TemporaryPiece->IterateTurn(1);
	// Place the piece in the new position.
	ChessBoard[NewX][NewY] = TemporaryPiece;
	// Set the old position to be a null pointer.
	ChessBoard[OldX][OldY] = nullptr;
	// If it was a pawn promotion move, promote the pawn.
	if (PromotePawnBool) { PromotePawn(NewX, NewY); }

}

// Function to move a piece and do nothing else.
void Board::MovePieceAndDoNothing(int OldX, int OldY, int NewX, int NewY)
{
	
	// Create a new temporary piece.
	shared_ptr<Piece> TemporaryPiece{ ChessBoard[OldX][OldY] };
	// Set the new position of the piece.
	TemporaryPiece->SetPosition(Point(NewX, NewY));
	// Place the piece in the new position.
	ChessBoard[NewX][NewY] = TemporaryPiece;
	// Set the old position to be a null pointer.
	ChessBoard[OldX][OldY] = nullptr;

}

// Function to check if the move is an en passant move.
bool Board::EnPassantMove(int OldX, int OldY, int NewX, int NewY)
{

	// Create a temporary piece.
	shared_ptr<Piece> TemporaryPiece{ ChessBoard[OldX][OldY] };
	// Check the long list of requirements.
	if (TemporaryPiece && TemporaryPiece->TypeOfMove(NewX, NewY) == PawnCapture && TemporaryPiece->GetName() == "Pawn" && !ChessBoard[NewX][NewY]) {
		if (NewX == 2 && TemporaryPiece->GetSymbol() == 'P' && ChessBoard[NewX + 1][NewY] && ChessBoard[NewX + 1][NewY]->GetSymbol() == 'p' && ChessBoard[NewX + 1][NewY]->GetTurn() == 1 && ChessBoard[NewX + 1][NewY]->GetLastPiece() == true) { return true; }
		if (NewX == 5 && TemporaryPiece->GetSymbol() == 'p' && ChessBoard[NewX - 1][NewY] && ChessBoard[NewX - 1][NewY]->GetSymbol() == 'P' && ChessBoard[NewX - 1][NewY]->GetTurn() == 1 && ChessBoard[NewX - 1][NewY]->GetLastPiece() == true) { return true; }
	}
	// If it doesn't meet all of the requirements, then return false.
	return false;

}

// Function to check if the move is a pawn promotion move.
bool Board::PawnPromotionMove(int OldX, int OldY, int NewX, int NewY)
{

	// Create a temporary piece.
	shared_ptr<Piece> TemporaryPiece{ ChessBoard[OldX][OldY] };
	// Check the long list of requirements.
	if (TemporaryPiece && TemporaryPiece->GetName() == "Pawn" && (TemporaryPiece->TypeOfMove(NewX, NewY) == PawnCapture || TemporaryPiece->TypeOfMove(NewX, NewY) == PawnMove) && TemporaryPiece->GetName() == "Pawn") {
		if (NewX == 0 && TemporaryPiece->GetColour() == "White") { return true; }
		if (NewX == 7 && TemporaryPiece->GetColour() == "Black") { return true; }
	}
	// If it doesn't meet all of the requirements, then return false.
	return false;

}

// Function to check if the move is a castling move.
bool Board::CastlingMove(int OldX, int OldY, int NewX, int NewY)
{

	// Create a temporary piece.
	shared_ptr<Piece> TemporaryPiece{ ChessBoard[OldX][OldY] };
	// Initialise some counter variables.
	int CounterRight{ 0 }, CounterLeft{ 0 };
	// Check the long list of requirements.
	if (TemporaryPiece && TemporaryPiece->TypeOfMove(NewX, NewY) == CastleRight && TemporaryPiece->GetName() == "King" && TemporaryPiece->GetTurn() == 0) {
		for (int i = 1; i < 3; i++) { if (ChessBoard[OldX][OldY + i]) { CounterRight++; } }
		if (CounterRight == 0 && ChessBoard[OldX][NewY + 1] && ChessBoard[OldX][NewY + 1]->GetTurn() == 0 && ChessBoard[OldX][NewY + 1]->GetName() == "Rook") { return true; }
	}
	if (TemporaryPiece && TemporaryPiece->TypeOfMove(NewX, NewY) == CastleLeft && TemporaryPiece->GetName() == "King" && TemporaryPiece->GetTurn() == 0) {
		for (int i = 1; i < 4; i++) { if (ChessBoard[OldX][OldY - i]) { CounterLeft++; } }
		if (CounterLeft == 0 && ChessBoard[OldX][NewY - 2] && ChessBoard[OldX][NewY - 2]->GetTurn() == 0 && ChessBoard[OldX][NewY - 2]->GetName() == "Rook") { return true; }
	}
	// If it doesn't meet all of the requirements, then return false.
	return false;

}

// Function to check if the piece at (OldX, OldY) can move to (NewX, NewY).
// The pieces tell the board how they can move and the board tells the pieces if they can move.
bool Board::CanPieceMove(int OldX, int OldY, int NewX, int NewY)
{

	// Create a temporary piece.
	shared_ptr<Piece> TemporaryPiece{ ChessBoard[OldX][OldY] };
	
	// If the coordinates are not on the board, return false.
	if (OldX < 0 || OldX >= 8 || OldY < 0 || OldY >= 8 || NewX < 0 || NewX >= 8 || NewY < 0 || NewY >= 8) { return false; }
	// Ensure the user can't move to the same position.
	if (OldX == NewX && OldY == NewY) { return false; }
	
	if (TemporaryPiece)
	{

		// If the piece would land on another piece of the same colour, return false.
		if (ChessBoard[NewX][NewY] && TemporaryPiece->GetColour() == ChessBoard[NewX][NewY]->GetColour()) { return false; }
		// If the type of move is an illegal one, return false.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == Illegal) { return false; }

		// If the type of move is a pawn move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == PawnMove)
		{
			// If the new coordinate is empty, return true, else return false.
			if (!ChessBoard[NewX][NewY]) { return true; }
			else { return false; }
		}

		// If the type of move is a double step, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == DoubleStep)
		{

			// Initialise a step counter.
			int Step{ 0 };
			// Change the step value according to the colour.
			if (TemporaryPiece->GetColour() == "White") { Step = -1; }
			if (TemporaryPiece->GetColour() == "Black") { Step =  1; }
			// If that fails for some reason, print an error message.
			if (TemporaryPiece->GetColour() != "White" && TemporaryPiece->GetColour() != "Black")
			{
				cerr << "Error: Could not determine the colour of the temporary piece." << endl;
			}

			// If neither of the two spaces in front of the pawn are occupied, return true.
			if (!ChessBoard[NewX][NewY] && !ChessBoard[NewX - Step][NewY]) { return true; }
			else { return false; }

		}

		// If the type of move is a double step, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == PawnCapture)
		{

			// Allow for the en passant possibilities.
			if (NewX == 2 && !ChessBoard[NewX][NewY] && ChessBoard[NewX + 1][NewY] && ChessBoard[NewX + 1][NewY]->GetSymbol() == 'p' && ChessBoard[NewX + 1][NewY]->GetTurn() == 1 && ChessBoard[NewX + 1][NewY]->GetLastPiece() == true) { return true; }
			if (NewX == 5 && !ChessBoard[NewX][NewY] && ChessBoard[NewX - 1][NewY] && ChessBoard[NewX - 1][NewY]->GetSymbol() == 'P' && ChessBoard[NewX - 1][NewY]->GetTurn() == 1 && ChessBoard[NewX - 1][NewY]->GetLastPiece() == true) { return true; }
			// Also allow for the regular scenario.
			if (ChessBoard[NewX][NewY]) { return true; }
			return false;

		}

		// If the type of move is a king move, return true as no further conditions need to be checked.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == KingMove) { return true; }
		
		// If the type of move is a castling right move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == CastleRight)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if the positions between the king and the rook are empty.
			for (int i = 1; i < 3; i++) { if (ChessBoard[OldX][OldY + i]) { Counter++; } }
			if (Counter == 0 && ChessBoard[OldX][NewY + 1] && ChessBoard[OldX][NewY + 1]->GetTurn() == 0 && ChessBoard[OldX][NewY + 1]->GetName() == "Rook") { return true; }
			else { return false; }

		}

		// If the type of move is a castling left move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == CastleLeft)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if the positions between the king and the rook are empty.
			for (int i = 1; i < 4; i++) { if (ChessBoard[OldX][OldY - i]) { Counter++; } }
			if (Counter == 0 && ChessBoard[OldX][NewY - 2] && ChessBoard[OldX][NewY - 2]->GetTurn() == 0 && ChessBoard[OldX][NewY - 2]->GetName() == "Rook") { return true; }
			else { return false; }

		}

		// If the type of move is a knight move, return true as no further conditions need to be checked.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == KnightMove) { return true; }

		// If the type of move is a straight up move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == Up)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < OldX - NewX; i++) { if (ChessBoard[OldX - i][OldY]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is a straight down move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == Down)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < NewX - OldX; i++) { if (ChessBoard[OldX + i][OldY]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is a straight right move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == Right)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < NewY - OldY; i++) { if (ChessBoard[OldX][OldY + i]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is a straight left move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == Left)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < OldY - NewY; i++) { if (ChessBoard[OldX][OldY - i]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is an up-right diagonal move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == DiagUR)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < OldX - NewX; i++) { if (ChessBoard[OldX - i][OldY + i]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is a down-left diagonal move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == DiagDL)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < NewX - OldX; i++) { if (ChessBoard[OldX + i][OldY - i]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is a down-right diagonal move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == DiagDR)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < NewY - OldY; i++) { if (ChessBoard[OldX + i][OldY + i]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		// If the move is an up-left diagonal move, enter this section.
		if (TemporaryPiece->TypeOfMove(NewX, NewY) == DiagUL)
		{

			// Initialise a counter.
			int Counter{ 0 };
			// Check if all the positions between the old and new position are empty.
			for (int i = 1; i < OldY - NewY; i++) { if (ChessBoard[OldX - i][OldY - i]) { Counter++; } }
			if (Counter == 0) { return true; }
			else { return false; }

		}

		return false;
	}

	else { return false; }

}

// Function to promote a pawn.
void Board::PromotePawn(int xCoordinate, int yCoordinate)
{

	// Extract the variables of the pawn.
	string Colour{ ChessBoard[xCoordinate][yCoordinate]->GetColour() };
	int    TurnNumber{ ChessBoard[xCoordinate][yCoordinate]->GetTurn() };
	bool   LastPiece{ ChessBoard[xCoordinate][yCoordinate]->GetLastPiece() };
	
	// Set the position to a null pointer.
	ChessBoard[xCoordinate][yCoordinate] = nullptr;
	// Now put either a white or black queen on the chessboard, as appropriate.
	if (Colour == "White") { ChessBoard[xCoordinate][yCoordinate] = make_shared<WhiteQueen>(xCoordinate, yCoordinate); }
	if (Colour == "Black") { ChessBoard[xCoordinate][yCoordinate] = make_shared<BlackQueen>(xCoordinate, yCoordinate); }
	// Set the turn number and last piece variable of the new queen.
	ChessBoard[xCoordinate][yCoordinate]->SetTurn(TurnNumber);
	ChessBoard[xCoordinate][yCoordinate]->SetLastPiece(LastPiece);

}

// Function to unpromote a pawn. This is useful when undoing moves.
void Board::UnPromotePawn(int xCoordinate, int yCoordinate)
{

	// Extract the variables of the queen.
	string Colour{ ChessBoard[xCoordinate][yCoordinate]->GetColour() };
	int    TurnNumber{ ChessBoard[xCoordinate][yCoordinate]->GetTurn() };
	bool   LastPiece{ ChessBoard[xCoordinate][yCoordinate]->GetLastPiece() };
	
	// Set the position to a null pointer.
	ChessBoard[xCoordinate][yCoordinate] = nullptr;
	// Now put either a white or black pawn on the chessboard, as appropriate.
	if (Colour == "White") { ChessBoard[xCoordinate][yCoordinate] = make_shared<WhitePawn>(xCoordinate, yCoordinate); }
	if (Colour == "Black") { ChessBoard[xCoordinate][yCoordinate] = make_shared<BlackPawn>(xCoordinate, yCoordinate); }
	// Set the turn number and last piece variable of the new pawn.
	ChessBoard[xCoordinate][yCoordinate]->SetTurn(TurnNumber);
	ChessBoard[xCoordinate][yCoordinate]->SetLastPiece(LastPiece);

}

// Function to see if the king is in check.
bool Board::KingInCheck(string Colour)
{

	// Create a new string that is the oppositions colour.
	string OppositionColour;
	// Set the oppositions colour as appropriate.
	if (Colour == "White") { OppositionColour = "Black"; }
	if (Colour == "Black") { OppositionColour = "White"; }
	// If the colour was not white or black, then print an error.
	if (Colour != "White" && Colour != "Black")
	{
		cerr << "Error: The colour of the player could not be identified." << endl;
	}

	// Declare the coordinates of the king piece.
	int KingXCoordinate, KingYCoordinate;

	// These loops find the position of the king.
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (ChessBoard[i][j] && ChessBoard[i][j]->GetName() == "King" && ChessBoard[i][j]->GetColour() == Colour) {
				KingXCoordinate = i;
				KingYCoordinate = j;
				// The break keyword is used so that once the function is finished...
				// it doesn't pointlessly loop through the rest of the coordinates.
				break;
			}
		}
	}

	// If an opposition piece can capture the king, return true.
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (ChessBoard[i][j] && ChessBoard[i][j]->GetColour() == OppositionColour && CanPieceMove(i, j, KingXCoordinate, KingYCoordinate))
			{
				return true;
			}
		}
	}
	
	// If the opposition piece can't capture the king, return false.
	return false;

}

// Function to set the variable 'LastPiece' to false for every piece.
void Board::SetBoardLastPieceFalse()
{

	// Iterate through all the pieces on the board.
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (ChessBoard[i][j])
			{
				// Set the last piece bool to false.
				ChessBoard[i][j]->SetLastPiece(false);
			}
		}
	}

}

// Function to return a quantifiable value of the strength of the chessboard.
double Board::EvaluateBoard()
{

	// Initialse the score.
	double Score{ 0 };

	// Iterate over the chessboard.
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (ChessBoard[i][j])
			{
				// Fetch the 2D position evaluation vector.
				vector<vector<double>> PosEvalVector{ ChessBoard[i][j]->PositionEvaluation() };
				if (ChessBoard[i][j]->GetColour() == "White")
				{
					// If the colour is white, just add the value and the vector element.
					Score += ChessBoard[i][j]->GetValue() + PosEvalVector[i][j];
				}
				else
				{
					// If the colour is black, we first need to rotate the 2D vector by 180 degrees...
					// because the vector is constructed for the white pieces only...
					// and then we subtract the vector element when calculating the score.
					Rotate(PosEvalVector);
					Score += ChessBoard[i][j]->GetValue() - PosEvalVector[i][j];
				}
			}
		}
	}

	// Return the score.
	return Score;

}

// Function to add a piece onto the chessboard.
void Board::AddPiece(shared_ptr<Piece> ChessPiece)
{
	ChessBoard[ChessPiece->GetPoint().GetX()][ChessPiece->GetPoint().GetY()] = ChessPiece;
}