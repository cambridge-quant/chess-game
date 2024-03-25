// OOP Chess Project: GameManager.cpp.
// This is the GameManager class source file.
// It contains all the definitions related to the GameManager class.
// James Cummins.

// Include the GameManager header file.
#include "GameManager.h"

// Using namespaces.
using namespace GameNamespace;

// Non member function to return the equivalent integer of a char.
// The value 97 appears because the char 'a' has an ASCII code of 97.
int ReturnNumber(char Input) { return (int)Input - 97; }

// Non member function to return the equivalent char of an integer.
char ReturnChar(int Input) { return (char)(Input + 97); }

// Non member function to return the file of a position on the chessboard.
int FileInput()
{

	// Declare the input.
	char InputFile;

	// While the input fails, or it's out of bounds, loop over this.
	while (!(cin >> InputFile) || ReturnNumber(tolower(InputFile)) < 0 || ReturnNumber(tolower(InputFile)) > 7)
	{
		// Print an error message.
		cerr << "Error: Input failed.\nPlease try again: ";
		// Clear the error state.
		cin.clear();
		// Ignore whatever the user entered.
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	// Clear the  input buffer.
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');

	// Return the good input as an integer.
	return ReturnNumber(tolower(InputFile));

}

// Non member function to return the rank of a position on the chessboard.
int GoodNumber()
{

	// Declare the input number.
	int InputNumber;

	// While the input fails, or it's out of bounds, enter this loop.
	while (!(cin >> InputNumber) || InputNumber < 1 || InputNumber > 8)
	{
		// Print an error message.
		cerr << "Error: Input failed.\nPlease try again: ";
		// Clear the error state.
		cin.clear();
		// Ignore whatever the user entered.
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	// Clear the input buffer.
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');

	// Return the good input.
	return InputNumber;

}

// Default constructor.
GameManager::GameManager()
{
	TheBoard->InitialiseBoard();
	GameTurnNumber = CaptureCounter = PawnMoveCounter = GameType = 0;
}

// Parameterised constructor.
GameManager::GameManager(Board *InputBoard)
{
	TheBoard = InputBoard;
	GameTurnNumber = CaptureCounter = PawnMoveCounter = GameType = 0;
}

// Access functions
int  GameManager::GetGameTurnNumber() const { return GameTurnNumber; }
bool GameManager::GetMaxBoardEval()   const { return MaximiseBoardEvaluation; }

// Function to inform the user that a pawn has been promoted.
void GameManager::PrintPawnPromotion()
{

	shared_ptr<Piece> MovedPiece{ TheBoard->GetComputerMovedPiece() };
	int MovedX{ TheBoard->GetComputerMovedX() };
	
	// If the promoted pawn is white, enter this section.
	if (MovedPiece->GetSymbol() == 'P' && MovedX == 0)
	{
		cout << "\nWhite Pawn has been promoted to White Queen." << endl;
	}
	// If the promoted pawn is black, enter this section.
	if (MovedPiece->GetSymbol() == 'p' && MovedX == 7)
	{
		cout << "\nBlack Pawn has been promoted to Black Queen." << endl;
	}

}

// Function to print the list of moves and save them to a file.
void GameManager::PrintList()
{

	// Print the list of moves from the multimap.
	for (const auto& it : ListOfMoves) { cout << it.first << ") " << it.second << endl; }

	// Creat the file to save the game to.
	ofstream myFile("ChessGame.txt");
	// Exception handling using the try and catch keywords
	try
	{

		// Get the current date and time.
		time_t rawtime;
		struct tm timeinfo;
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		
		// Print the title, date, and time to the file.
		myFile << "Chess Game by James Cummins." << endl;
		myFile << "Date: " << timeinfo.tm_mday << "/" << (timeinfo.tm_mon + 1) << "/" << (timeinfo.tm_year + 1900) << endl;
		myFile << "Time: " << timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << "\n" << endl;
		
		// Print the lise of moves to the file.
		for (const auto& it : ListOfMoves) { myFile << it.first << ") " << it.second << endl; }

		// Close the file.
		myFile.close();

	}
	// If it failed, print an appropriate error message.
	catch (exception& e) { cout << "Error: " << e.what() << endl; }

}

// Function to make a human move.
void GameManager::HumanMove()
{

	// Declare some useful variables.
	string Colour1, Colour2;
	bool GoodHumanInput{ false };

	// Set Colour1 and Colour2 according to whose turn it is.
	// An even GameTurnNumber corresponds to white's turn, and odd corresponds to black's turn.
	if (GameTurnNumber % 2 == 0) { Colour1 = "White"; Colour2 = "Black"; }
	else { Colour2 = "White"; Colour1 = "Black"; }

	// Print a message of whose turn it is.
	if (Colour1 == "White") { cout << "\n" << (GameTurnNumber) / 2 + 1 << ") White Turn." << endl; }
	if (Colour1 == "Black") { cout << "\n" << (GameTurnNumber + 1) / 2 << ") Black Turn." << endl; }

	// Get the current time to measure the elapsed time of the move.
	auto Start = chrono::system_clock::now();

	// While the GoodHumanInput bool is false, do this loop.
	while (!GoodHumanInput)
	{

		// Ask the user to enter the coordinates.
		cout << "\nEnter the current file of the piece you'd like to move (e.g. 'a') : ";
		int OldY{ FileInput() };
		cout << "Enter the current rank of the piece you'd like to move (e.g. '1') : ";
		int OldX{ 8 - GoodNumber() };
		cout << "Enter the file of the position you'd like to move the piece to    : ";
		int NewY{ FileInput() };
		cout << "Enter the rank of the position you'd like to move the piece to    : ";
		int NewX{ 8 - GoodNumber() };

		// If the coordinates correspond to a valid move, enter this section.
		if (TheBoard->GetPiece(OldX, OldY) && TheBoard->GetPiece(OldX, OldY)->GetColour() == Colour1 && TheBoard->CanPieceMove(OldX, OldY, NewX, NewY) && WillPieceAvoidCheckMate(OldX, OldY, NewX, NewY, Colour1)) {
			
			// If the moved piece was a pawn, then reset the PawnMoveCounter to zero. Else, iterate it by one.
			if (TheBoard->GetPiece(OldX, OldY)->GetName() == "Pawn") { PawnMoveCounter = 0; }
			else { PawnMoveCounter++; }

			// If the turn colour is white, then insert the move into the multimap with the correct adjusted GameTurnNumber.
			if (Colour1 == "White")
			{
				ListOfMoves.insert(pair <int, string> ((GameTurnNumber)/2 + 1, TheBoard->GetPiece(OldX, OldY)->GetColour() + " " + TheBoard->GetPiece(OldX, OldY)->GetName() + " was moved from (" + ReturnChar(OldY) + ", " + to_string(8 - OldX) + ") to (" + ReturnChar(NewY) + ", " + to_string(8 - NewX) + ")."));
			}
			// Else if the colour is black, then do the same thing with the different adjusted GameTurnNumber.
			else
			{
				ListOfMoves.insert(pair <int, string>((GameTurnNumber + 1) / 2, TheBoard->GetPiece(OldX, OldY)->GetColour() + " " + TheBoard->GetPiece(OldX, OldY)->GetName() + " was moved from (" + ReturnChar(OldY) + ", " + to_string(8 - OldX) + ") to (" + ReturnChar(NewY) + ", " + to_string(8 - NewX) + ")."));
			}
			// Add the coordinates to the SavedGame vector.
			SavedGame.push_back({ OldX, OldY, NewX, NewY });

			// If the move is an en passant move, print out a personalised message.
			if (TheBoard->EnPassantMove(OldX, OldY, NewX, NewY)) {
				cout << "\nCongratulations! " << Colour1 << " Pawn took " << Colour2 << " Pawn via en passant." << endl;
				CaptureCounter = 0;
			}
			// If the move captured a piece, print out a personalised message.
			if (TheBoard->GetPiece(NewX, NewY)) {
				cout << "\nCongratulations! " << Colour1 << " " << TheBoard->GetPiece(OldX, OldY)->GetName() << " took " << Colour2 << " " << TheBoard->GetPiece(NewX, NewY)->GetName() << "." << endl;
				CaptureCounter = 0;
			}
			// If no piece was captured, iterate the capture counter by one.
			if (!TheBoard->EnPassantMove(OldX, OldY, NewX, NewY) && !TheBoard->GetPiece(NewX, NewY)) { CaptureCounter++; }

			// Set the computer pieces to null pointers.
			TheBoard->SetComputerEnPassantPiece(nullptr);
			TheBoard->SetComputerPromotedPawn(nullptr);

			// Move the piece.
			TheBoard->MovePiece(OldX, OldY, NewX, NewY);
			// If the move was a pawn promotion, this function will print out a message saying so.
			PrintPawnPromotion();

			// Set the variable LastPiece to false for all the piece.
			TheBoard->SetBoardLastPieceFalse();
			// Then set LastPiece to true for the piece that was just moved.
			TheBoard->GetPiece(NewX, NewY)->SetLastPiece(true);

			// To get here the input was good so set the GoodHumanInput bool to true.
			GoodHumanInput = true;
		}
		// If it was not a valid move, print out a message.
		else { cout << "Error: Invalid " << Colour1 << " move.\nPlease try again." << endl; }

	}

	// Get the current time now that the move has finished.
	auto End = chrono::system_clock::now();
	// Print out the elapsed time in seconds to two decimal places.
	cout << fixed << setprecision(2) << "\nElapsed time: " << chrono::duration<double>(End - Start).count() << " seconds." << endl;

	// Print the updated chessboard.
	TheBoard->PrintBoard();
	// Print a warning message if the opponent's king is in check.
	if (TheBoard->KingInCheck(Colour2) && !KingInCheckMate(Colour2)) { cout << "\nWarning: " << Colour2 << " King is in check." << endl; }

	// Iterate the GameTurnNumber.
	GameTurnNumber++;
	// Reset the computer en passant piece to a null pointer.
	TheBoard->SetComputerEnPassantPiece(nullptr);

}

// Function to make a computer move.
void GameManager::ComputerMove(bool IntelligentTrueRandomFalse)
{
	
	// Declare some useful variables.
	string Colour1, Colour2;
	bool GoodComputerInput{ false };
	int OldX, OldY, NewX, NewY;
	PossibleMove AIMove;

	// Set Colour1 and Colour2 according to whose turn it is.
	if (GameTurnNumber % 2 == 0) { Colour1 = "White"; Colour2 = "Black"; }
	else { Colour2 = "White"; Colour1 = "Black"; }

	// Print a message of whose turn it is.
	if (Colour1 == "White") { cout << "\n" << (GameTurnNumber) / 2 + 1 << ") White Turn." << endl; }
	if (Colour1 == "Black") { cout << "\n" << (GameTurnNumber + 1) / 2 << ") Black Turn." << endl; }

	// Get the current time to measure the elapsed time of the move.
	auto Start = chrono::system_clock::now();

	// While the computer input is not good, loop this.
	while (!GoodComputerInput)
	{

		// If we want an intelligent (AI) move, enter this section.
		if (IntelligentTrueRandomFalse)
		{
			// **************************** MINIMAX INSTRUCTIONS *******************************
			// Depending on the turn colour, we want to either maximise or minimise the board evaluation,
			// which is why the MaximiseBoardEvaluation bool is set differently depending on the colour.
			// The default search depth I have on here is two (RECOMMENDED, ~ 1-10 seconds).
			// You can turn the depth down to one which is very fast (~ 0.1-0.5 seconds).
			// If you want the AI to be better then you can increase the depth to three,
			// (On my very old laptop, three is the furthest I dare go)! (~ 0.5-2.0 mins).
			// A depth further than three will really take too long so I wouldn't do that.
			// Note: Depth can be 1,2,3,4,... (NOT ZERO).
			const int MinimaxDepth{ 2 }; // <- Set minimax depth here.
			if (Colour1 == "White") { MaximiseBoardEvaluation = true; AIMove = MinimaxMove(MinimaxDepth, true); }
			else { MaximiseBoardEvaluation = false; AIMove = MinimaxMove(MinimaxDepth, false); }
		}
		// Else if we just want a random move.
		else
		{
			// Get a vector of all the possible moves that can be made.
			vector<PossibleMove> GameMoves{ AllPossibleMoves(Colour1) };
			// Randomly shuffle the vector.
			random_shuffle(GameMoves.begin(), GameMoves.end());
			// Select the first element of the vector. This will then be a randomly selected move.
			AIMove = GameMoves[0];
		}

		// Set the coordinates to what the computer has come up with.
		OldX = AIMove.OriginalX;
		OldY = AIMove.OriginalY;
		NewX = AIMove.MovedX;
		NewY = AIMove.MovedY;

		// Double check to make sure it's a valid move.
		if (TheBoard->GetPiece(OldX, OldY) && TheBoard->GetPiece(OldX, OldY)->GetColour() == Colour1 && TheBoard->CanPieceMove(OldX, OldY, NewX, NewY) && WillPieceAvoidCheckMate(OldX, OldY, NewX, NewY, Colour1))
		{
			GoodComputerInput = true;
		}

	}

	// If the moved piece was a pawn, then reset the PawnMoveCounter to zero. Else, iterate it by one.
	if (TheBoard->GetPiece(OldX, OldY)->GetName() == "Pawn") { PawnMoveCounter = 0; }
	else { PawnMoveCounter++; }

	// If the turn colour is white, then insert the move into the multimap with the correct adjusted GameTurnNumber.
	if (Colour1 == "White")
	{
		ListOfMoves.insert(pair <int, string>((GameTurnNumber) / 2 + 1, TheBoard->GetPiece(OldX, OldY)->GetColour() + " " + TheBoard->GetPiece(OldX, OldY)->GetName() + " was moved from (" + ReturnChar(OldY) + ", " + to_string(8 - OldX) + ") to (" + ReturnChar(NewY) + ", " + to_string(8 - NewX) + ")."));
	}
	// Else if the colour is black, then do the same thing with the different adjusted GameTurnNumber.
	else
	{
		ListOfMoves.insert(pair <int, string>((GameTurnNumber + 1) / 2, TheBoard->GetPiece(OldX, OldY)->GetColour() + " " + TheBoard->GetPiece(OldX, OldY)->GetName() + " was moved from (" + ReturnChar(OldY) + ", " + to_string(8 - OldX) + ") to (" + ReturnChar(NewY) + ", " + to_string(8 - NewX) + ")."));
	}
	// Add the coordinates to the SavedGame vector.
	SavedGame.push_back({ OldX, OldY, NewX, NewY });

	// If the move is an en passant move, print out a personalised message.
	if (TheBoard->EnPassantMove(OldX, OldY, NewX, NewY)) {
		cout << "\n" << Colour1 << " Pawn took " << Colour2 << " Pawn via en passant." << endl;
		CaptureCounter = 0;
	}
	// If the move captured a piece, print out a personalised message.
	if (TheBoard->GetPiece(NewX, NewY)) {
		cout << "\n" << Colour1 << " " << TheBoard->GetPiece(OldX, OldY)->GetName() << " took " << Colour2 << " " << TheBoard->GetPiece(NewX, NewY)->GetName() << "." << endl;
		CaptureCounter = 0;
	}
	// If no piece was captured, iterate the capture counter by one.
	if (!TheBoard->EnPassantMove(OldX, OldY, NewX, NewY) && !TheBoard->GetPiece(NewX, NewY)) { CaptureCounter++; }

	// Move the piece.
	TheBoard->MovePiece(OldX, OldY, NewX, NewY);
	// If the move was a pawn promotion, this function will print out a message saying so.
	PrintPawnPromotion();

	// Set the variable LastPiece to false for all the piece.
	TheBoard->SetBoardLastPieceFalse();
	// Then set LastPiece to true for the piece that was just moved.
	TheBoard->GetPiece(NewX, NewY)->SetLastPiece(true);

	// Get the current time now that the move has finished.
	auto End = chrono::system_clock::now();
	// Print out the elapsed time in seconds or milliseconds to two decimal places.
	if (IntelligentTrueRandomFalse)
	{
		cout << fixed << setprecision(2) << "\nElapsed time: " << chrono::duration<double>(End - Start).count() << " seconds." << endl;
	}
	else
	{
		cout << fixed << setprecision(2) << "\nElapsed time: " << 1000 * chrono::duration<double>(End - Start).count() << " milliseconds." << endl;
	}

	// Print the updated chessboard.
	TheBoard->PrintBoard();
	// Print a warning message if the opponent's king is in check.
	if (TheBoard->KingInCheck(Colour2) && !KingInCheckMate(Colour2)) { cout << "\nWarning: " << Colour2 << " King is in check." << endl; }

	// Iterate the GameTurnNumber.
	GameTurnNumber++;

}

// Function to make a specified move.
void GameManager::SpecifiedMove(int OldX, int OldY, int NewX, int NewY, bool Save)
{

	// If the moved piece was a pawn, then reset the PawnMoveCounter to zero. Else, iterate it by one.
	if (TheBoard->GetPiece(OldX, OldY)->GetName() == "Pawn") { PawnMoveCounter = 0; }
	else { PawnMoveCounter++; }

	// If the turn colour is white, then insert the move into the multimap with the correct adjusted GameTurnNumber.
	if (GameTurnNumber % 2 == 0)
	{
		ListOfMoves.insert(pair <int, string>((GameTurnNumber) / 2 + 1, TheBoard->GetPiece(OldX, OldY)->GetColour() + " " + TheBoard->GetPiece(OldX, OldY)->GetName() + " was moved from (" + ReturnChar(OldY) + ", " + to_string(8 - OldX) + ") to (" + ReturnChar(NewY) + ", " + to_string(8 - NewX) + ")."));
	}
	// Else if the colour is black, then do the same thing with the different adjusted GameTurnNumber.
	else
	{
		ListOfMoves.insert(pair <int, string>((GameTurnNumber + 1) / 2, TheBoard->GetPiece(OldX, OldY)->GetColour() + " " + TheBoard->GetPiece(OldX, OldY)->GetName() + " was moved from (" + ReturnChar(OldY) + ", " + to_string(8 - OldX) + ") to (" + ReturnChar(NewY) + ", " + to_string(8 - NewX) + ")."));
	}
	// Add the coordinates to the SavedGame vector if specified.
	if (Save) { SavedGame.push_back({ OldX, OldY, NewX, NewY }); }

	// If the move captured an opposing piece, reset CaptureCounter to zero.
	if (TheBoard->EnPassantMove(OldX, OldY, NewX, NewY)) { CaptureCounter = 0; }
	if ( TheBoard->GetPiece( NewX , NewY ) )             { CaptureCounter = 0; }
	// If no piece was captured, iterate the capture counter by one.
	if (!TheBoard->EnPassantMove(OldX, OldY, NewX, NewY) && !TheBoard->GetPiece(NewX, NewY)) { CaptureCounter++; }

	// Move the piece.
	TheBoard->MovePiece(OldX, OldY, NewX, NewY);
	// Set the variable LastPiece to false for all the piece.
	TheBoard->SetBoardLastPieceFalse();
	// Then set LastPiece to true for the piece that was just moved.
	TheBoard->GetPiece(NewX, NewY)->SetLastPiece(true);
	// Iterate the GameTurnNumber.
	GameTurnNumber++;

}

// Function to give the user a choice of game modes.
int  GameManager::GameModeSelection()
{

	// Inform the user of the game modes to choose from.
	cout << "\nSelect Game Mode:" << endl;
	cout << "[1] Human VS Human" << endl;
	cout << "[2] Human VS Computer (Random)" << endl;
	cout << "[3] Human VS Computer (Intelligent)" << endl;
	cout << "[4] Computer (Intelligent) VS Computer (Random).\n" << endl;
	cout << "Please enter the corresponding number: ";

	// Get an input for the game mode, using the template function.
	int Mode{ GoodInput(1, 2, 3, 4) };

	// Return the corresponding game mode.
	if (Mode == 1) { return 1; }
	if (Mode == 2) { return 2; }
	if (Mode == 3) { return 3; }
	if (Mode == 4) { return 4; }

	// As a backup, if the good input function fails (which it definetly shouldn't)...
	// then print an error message and return game mode 1.
	if (Mode != 1 && Mode != 2 && Mode != 3 && Mode != 4)
	{
		cerr << "Error: Could not determine game mode.\nDefault selected." << endl;
	}
	
	return 1;

}

// Function to give the user a choice to load a saved game.
void GameManager::LoadGameSelection()
{

	// Ask the user if they would like to load a game.
	cout << "\nWould you like to load a previously saved game (Y/N)? ";

	// Use the template function to get the user's input.
	char Answer{ GoodInput('Y', 'Y', 'N', 'N') };

	// If the answer was a yes, then enter this section.
	if (Answer == 'Y')
	{
		// Load the game.
		cout << "\nLoading game..." << endl;
		LoadGame();
		// Print out who just had their turn.
		if (GameTurnNumber != 0)
		{
			if (GameTurnNumber % 2 == 0) { cout << "\n" << GameTurnNumber / 2 << ") Black Turn." << endl; }
			else { cout << "\n" << (GameTurnNumber + 1) / 2 << ") White Turn." << endl; }
		}
		// Print the chessboard.
		TheBoard->PrintBoard();
	}
	// If the answer was a no then do nothing.

}

// Function to give the user options before making a move.
void GameManager::BeginOptionMenu()
{

	// Define and set the colour.
	string Colour;
	if (GameTurnNumber % 2 == 0) { Colour = "White"; }
	else { Colour = "Black"; }

	// Print out the available options.
	cout << "\nOptions: [P]lay next " << Colour << " move. Display " << Colour << " [A]llowed moves. [H]int. [Q]uit game." << endl;
	
	// Get the chosen option using the template function.
	char ChosenOption{ GoodInput('P', 'A', 'H', 'Q') };

	// If Q, then quit the game.
	if (ChosenOption == 'Q') { cout << "Thank you for playing." << endl; exit(1); }
	// If A, then print the allowed moves.
	if (ChosenOption == 'A') { PrintAllowedMoves(Colour); }
	// If H, then print out the best move.
	if (ChosenOption == 'H')
	{
		cout << "Calculating best move..." << endl;
		PossibleMove SuggestedMove;
		// Default search depth here is two.
		if (Colour == "White") { MaximiseBoardEvaluation = true; SuggestedMove = MinimaxMove(2, true); }
		else { MaximiseBoardEvaluation = false; SuggestedMove = MinimaxMove(2, false); }
		cout << "Suggested move: (" << ReturnChar(SuggestedMove.OriginalY) << ", " << 8 - SuggestedMove.OriginalX << ") -> (" << ReturnChar(SuggestedMove.MovedY) << ", " << 8 - SuggestedMove.MovedX << ")." << endl;
	}

}

// Function to give the user options after  making a move.
void GameManager::EndOptionMenu()
{

	// Define and set the colour.
	string Colour;
	if (GameTurnNumber % 2 == 0) { Colour = "White"; }
	else { Colour = "Black"; }

	// Print out the available options.
	cout << "\nOptions: [G]ive turn over to " << Colour << ". [U]ndo last move. [S]ave game. [Q]uit game." << endl;

	// Get the chosen option using the template function.
	char ChosenOption{ GoodInput('G', 'U', 'S', 'Q') };

	// If S, save the game.
	if (ChosenOption == 'S') { cout << "Game saved to file (SavedGame.txt)." << endl; SaveGame(); }
	// If Q, quit the game.
	if (ChosenOption == 'Q') { cout << "Thank you for playing." << endl; exit(1); }
	// If U, undo the last move, and print the updated chessboard.
	if (ChosenOption == 'U')
	{
		cout << "Undoing last move." << endl;
		UndoAnyNumberOfMoves(1);
		TheBoard->PrintBoard();
	}

}

// Function to print all the allowed moves.
void GameManager::PrintAllowedMoves(string Colour)
{

	// Print the number of allowed moves.
	cout << "\n" << NumberOfAllowedMoves(Colour) << " allowed moves:" << endl;

	// Iterate through the entire chessboard checking to see if a move can be made.
	// If the move results in a capture, then it will inform the user of this.
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// Conditions occur at the earliest opportunity to increase the speed of the code.
			if (TheBoard->GetPiece(Point(i, j)) && TheBoard->GetPiece(Point(i, j))->GetColour() == Colour) {
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						if (TheBoard->CanPieceMove(i, j, k, l) && WillPieceAvoidCheckMate(i, j, k, l, Colour)) {
							if (TheBoard->EnPassantMove(i, j, k, l) && k == 2) { cout << "White Pawn at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) << ", 5) can capture the Black Pawn at (" << ReturnChar(l) << ", 5) via en passant by moving to (" << ReturnChar(l) << ", 6)." << endl; }
							if (TheBoard->EnPassantMove(i, j, k, l) && k == 5) { cout << "Black Pawn at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) << ", 4) can capture the White Pawn at (" << ReturnChar(l) << ", 4) via en passant by moving to (" << ReturnChar(l) << ", 3)." << endl; }
							if (TheBoard->CastlingMove(i, j, k, l) && k == 7) { cout << "White King at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) << ", 1) can move to (" << ReturnChar(l) << ", 1) via castling." << endl; }
							if (TheBoard->CastlingMove(i, j, k, l) && k == 0) { cout << "Black King at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) << ", 8) can move to (" << ReturnChar(l) << ", 8) via castling." << endl; }
							if (TheBoard->GetPiece(Point(i, j))->GetName() == "Pawn" && (k == 0 || k == 7)) {
								if (TheBoard->GetPiece(Point(k, l))) {
									cout << Colour << " " << TheBoard->GetPiece(Point(i, j))->GetName() << " at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) << ", " <<
										8 - TheBoard->GetPiece(Point(i, j))->GetPoint().GetX() << ") can capture the " << TheBoard->GetPiece(Point(k, l))->GetColour() << " " <<
										TheBoard->GetPiece(Point(k, l))->GetName() << " at (" << ReturnChar(l) << ", " << 8 - k << ") and be promoted to Queen." << endl;
								}
								else {
									cout << Colour << " " << TheBoard->GetPiece(Point(i, j))->GetName() << " at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) <<
										", " << 8 - TheBoard->GetPiece(Point(i, j))->GetPoint().GetX() << ") can move to (" << ReturnChar(l) << ", " << 8 - k << ") and be promoted to Queen." << endl;
								}
							}
							else if (!TheBoard->EnPassantMove(i, j, k, l) && !TheBoard->CastlingMove(i, j, k, l)) {
								if (TheBoard->GetPiece(Point(k, l))) {
									cout << Colour << " " << TheBoard->GetPiece(Point(i, j))->GetName() << " at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) << ", " <<
										8 - TheBoard->GetPiece(Point(i, j))->GetPoint().GetX() << ") can capture the " << TheBoard->GetPiece(Point(k, l))->GetColour() << " " <<
										TheBoard->GetPiece(Point(k, l))->GetName() << " at (" << ReturnChar(l) << ", " << 8 - k << ")." << endl;
								}
								else {
									cout << Colour << " " << TheBoard->GetPiece(Point(i, j))->GetName() << " at (" << ReturnChar(TheBoard->GetPiece(Point(i, j))->GetPoint().GetY()) <<
										", " << 8 - TheBoard->GetPiece(Point(i, j))->GetPoint().GetX() << ") can move to (" << ReturnChar(l) << ", " << 8 - k << ")." << endl;
								}
							}
						}
					}
				}
			}
		}
	}

}

// Function to return the number of allowed moves.
int  GameManager::NumberOfAllowedMoves(string Colour)
{

	// Initialise the number of allowed moves to zero.
	int AllowedMoves{ 0 };

	// Iterate through the entire chessboard checking all the moves that can be made.
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// Conditions occur at the earliest opportunity to increase the speed of the code.
			if (TheBoard->GetPiece(Point(i, j)) && TheBoard->GetPiece(Point(i, j))->GetColour() == Colour)
			{
				for (int k = 0; k < 8; k++)
				{
					for (int l = 0; l < 8; l++)
					{
						if (TheBoard->CanPieceMove(i, j, k, l) && WillPieceAvoidCheckMate(i, j, k, l, Colour))
						{
							// If the move is allowed, iterate the counter by one.
							AllowedMoves++;
						}
					}
				}
			}
		}
	}

	// Return the number of allowed moves.
	return AllowedMoves;

}

// Function to return a vector of all the allowed moves.
vector<PossibleMove> GameManager::AllPossibleMoves(string Colour)
{

	// Define the vector of all possible moves.
	vector<PossibleMove> PossibleMovesVector;

	// Iterate through the entire chessboard checking all the moves that can be made.
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// Conditions occur at the earliest opportunity to increase the speed of the code.
			if (TheBoard->GetPiece(Point(i, j)) && TheBoard->GetPiece(Point(i, j))->GetColour() == Colour)
			{
				for (int k = 0; k < 8; k++)
				{
					for (int l = 0; l < 8; l++)
					{
						if (TheBoard->CanPieceMove(i, j, k, l) && WillPieceAvoidCheckMate(i, j, k, l, Colour))
						{
							// If the move is allowed, add it to the vector.
							PossibleMovesVector.push_back({i, j, k, l});
						}
					}
				}
			}
		}
	}

	// Return the completed vector.
	return PossibleMovesVector;

}

// Function to check if the king is in checkmate or not.
bool GameManager::KingInCheckMate(string Colour)
{

	// If the number of allowed moves is zero then the king is in checkmate.
	// Recall that an allowed move is one in which the king does not end up in checkmate.
	if (NumberOfAllowedMoves(Colour) == 0) { return true; }
	else { return false; }

}

// Function to check if the fifty move rule has been satisfied.
bool GameManager::FiftyMoveRule()
{

	// If the capture counter and the pawn move counter are greater than...
	// or equal to one hundred then return true.
	if (CaptureCounter >= 100 && PawnMoveCounter >= 100) { return true; }
	else { return false; }

}

// Function to check if the game is over or not.
bool GameManager::IsGameOver()
{

	// Define and set the colours.
	string Colour1, Colour2;
	if (GameTurnNumber % 2 == 0) { Colour1 = "White"; Colour2 = "Black"; }
	else { Colour2 = "White"; Colour1 = "Black"; }

	// If the king is in checkmate then print that out and return true.
	if (KingInCheckMate(Colour1))
	{
		cout << "\n" << Colour1 << " King is in check mate.\n" << Colour2 << " wins." << endl;
		return true;
	}
	// If the fifty move rule is satisfied then print that out and return true.
	if (FiftyMoveRule())
	{
		cout << "\n50 moves have elapsed without a capture or a pawn move.\nThe game ends in a draw." << endl;
		return true;
	}
	// Else return false.
	return false;

}

// Function to see if a move can be made without putting the king in check mate.
bool GameManager::WillPieceAvoidCheckMate(int OldX, int OldY, int NewX, int NewY, string Colour)
{
	// Set the relevant pieces and make the move.
	if (!TheBoard->EnPassantMove(OldX, OldY, NewX, NewY)) {
		TheBoard->SetComputerEnPassantPiece(nullptr);
		TheBoard->MovePiece(OldX, OldY, NewX, NewY);
		TheBoard->SetComputerEnPassantPiece(nullptr);
	}
	else { TheBoard->MovePiece(OldX, OldY, NewX, NewY); }

	// If the king is now in check then undo that last move and return false.
	if (TheBoard->KingInCheck(Colour))
	{
		UndoLastComputerMove();
		return false;
	}
	// Else, undo that last move and return true.
	else
	{
		UndoLastComputerMove();
		return true;
	}

}

// Function to undo a move made by the computer.
// This is very useful when wanting to quickly undo just one move.
void GameManager::UndoLastComputerMove()
{

	// Set the relevant pieces.
	shared_ptr<Piece> ComputerMovedPiece{ TheBoard->GetComputerMovedPiece() };
	shared_ptr<Piece> ComputerCapturedPiece{ TheBoard->GetComputerCapturedPiece() };
	shared_ptr<Piece> ComputerEnPassantPiece{ TheBoard->GetComputerEnPassantPiece() };
	shared_ptr<Piece> ComputerPromotedPawn{ TheBoard->GetComputerPromotedPawn() };

	// Set the relevant coordinates.
	int OriginalX{ TheBoard->GetComputerOriginalX() };
	int OriginalY{ TheBoard->GetComputerOriginalY() };
	int MovedX{ TheBoard->GetComputerMovedX() };
	int MovedY{ TheBoard->GetComputerMovedY() };

	// Iterate the turn by minus two.
	// The net effect will be to decrease the turn by one since we will now move this piece back to where it was.
	ComputerMovedPiece->IterateTurn(-2);

	// Move the castling pieces back and do nothing else.
	if (ComputerMovedPiece->GetName() == "King" && OriginalX == MovedX && MovedY - OriginalY == 2) { TheBoard->MovePieceAndDoNothing(OriginalX, MovedY - 1, OriginalX, MovedY + 1); }
	if (ComputerMovedPiece->GetName() == "King" && OriginalX == MovedX && MovedY - OriginalY == -2) { TheBoard->MovePieceAndDoNothing(OriginalX, MovedY + 1, OriginalX, MovedY - 2); }
	
	// Undo the pawn promotion if it was a pawn promotion move.
	if (ComputerPromotedPawn) { TheBoard->UnPromotePawn(MovedX, MovedY); }

	// Move the piece back to where it was.
	TheBoard->MovePiece(MovedX, MovedY, OriginalX, OriginalY);

	// Add the captured piece back to the board if a piece was captured.
	if (ComputerCapturedPiece) { TheBoard->AddPiece(ComputerCapturedPiece); }
	// Add the captured en passant piece back to the board if a piece was captured via en passant.
	if (ComputerEnPassantPiece) { TheBoard->AddPiece(ComputerEnPassantPiece); }
	
	// Now set these computer pieces to null pointers.
	TheBoard->SetComputerPromotedPawn(nullptr);
	TheBoard->SetComputerEnPassantPiece(nullptr);

}

// Function to undo any number of specified moves.
// This is very useful when undoing human and minimax moves.
void GameManager::UndoAnyNumberOfMoves(int Number)
{

	// Declare a back up game as a vector of moves.
	vector<PossibleMove> BackUpGame;

	// Delete the specified number of elements in the saved game vector.
	if (!SavedGame.empty())
	{
		for (int i = 0; i < Number; i++)
		{
			SavedGame.pop_back();
		}
	}

	// Initialise the board.
	TheBoard->InitialiseBoard();
	// Set all the variables to zero.
	GameTurnNumber = CaptureCounter = PawnMoveCounter = 0;
	// Clear the list of moves.
	ListOfMoves.clear();
	// Set the back up game as the edited saved game.
	BackUpGame = SavedGame;
	// Clear the saved game.
	SavedGame.clear();

	// If the back up game is not empty...
	// iterate through it to redo the entire game minus the number of moves removed.
	if (!BackUpGame.empty())
	{
		// Use a lambda function to iterate through the back up game.
		for_each(BackUpGame.begin(), BackUpGame.end(), [&](PossibleMove &TheMove)
		{
			// Make each move as it was made.
			SpecifiedMove(TheMove.OriginalX, TheMove.OriginalY, TheMove.MovedX, TheMove.MovedY, true);
		});
	}

}

// Function to save the current game.
void GameManager::SaveGame()
{

	// Create the text file to save the game to.
	ofstream myFile("SavedGame.txt");

	// If the file is open, enter this section.
	if (myFile.is_open())
	{
		// Use a lambda function to iterate through the saved game.
		for_each(SavedGame.begin(), SavedGame.end(), [&](PossibleMove &TheMove)
		{
			// Print each move to the file.
			myFile << TheMove.OriginalX << " " << TheMove.OriginalY << " " << TheMove.MovedX << " " << TheMove.MovedY << endl;
		});
		// Close the file.
		myFile.close();
	}
	// Otherwise print an error message.
	else { cerr << "Error: Unable to open file for saving." << endl; }

}

// Function to load a saved game.
void GameManager::LoadGame()
{

	// Set all the variables to zero.
	GameTurnNumber = CaptureCounter = PawnMoveCounter = 0;
	// Find the saved game file.
	ifstream theFile("SavedGame.txt");
	// Define a temporary move.
	PossibleMove TemporaryMove;
	// Clear the saved game vector (it should be empty already anyway) 
	SavedGame.clear();

	// Exception handling.
	// This is very useful if the file has been corrupted.
	try {
		// If the file can be found and opened, enter this section.
		if (theFile.is_open())
		{

			// While we're not at the end of the file.
			while (!theFile.eof())
			{
				// Read in the move and add it to the SavedGame vector.
				theFile >> TemporaryMove.OriginalX >> TemporaryMove.OriginalY >> TemporaryMove.MovedX >> TemporaryMove.MovedY;
				SavedGame.push_back(TemporaryMove);
			}
			// Close the file
			theFile.close();
			// Delete the last element because it duplicates it.
			SavedGame.pop_back();

			// Use a lambda function to iterate through the saved game.
			for_each(SavedGame.begin(), SavedGame.end(), [&](PossibleMove &TheMove)
			{
				// Make each move as it was made.
				SpecifiedMove(TheMove.OriginalX, TheMove.OriginalY, TheMove.MovedX, TheMove.MovedY, false);
			});
		}
		// Otherwise print an error message.
		else { cerr << "Error: Unable to open file for loading." << endl; }
	}
	// If it failed, it will print an appropriate error message.
	catch (exception& e) { cout << "Error: " << e.what() << endl; }

}

// Function to return the best move via the minimax algorithm.
PossibleMove GameManager::MinimaxMove(int Depth, bool Maximise)
{

	// Make sure the depth isn't less than or equal to zero.
	if (Depth <= 0) { cout << "Error: Minimax depth cannot be zero." << endl; exit(1); }

	// Define and set the colour.
	string Colour;
	if (Maximise) { Colour = "White"; }
	else { Colour = "Black"; }

	// Initialise the best score to a very large negative number.
	double BestScore{ -9999 };
	// Declare the best move.
	PossibleMove BestMove;

	// Get the vector of all possible moves.
	vector<PossibleMove> GameMoves{ AllPossibleMoves(Colour) };
	// Shuffle the vector to avoid the same moves being played.
	random_shuffle(GameMoves.begin(), GameMoves.end());

	// Iterate through the possible moves.
	for (auto it = GameMoves.begin(); it != GameMoves.end(); it++)
	{
		PossibleMove TheMove = *it;
		// Make the possible move.
		SpecifiedMove(TheMove.OriginalX, TheMove.OriginalY, TheMove.MovedX, TheMove.MovedY, true);
		// Use recursion to get the value of the state of the board.
		double Value{ Minimax(Depth - 1, -10000, 10000, !Maximise) };
		// Undo that last move.
		UndoAnyNumberOfMoves(1);
		// If the value is better than or equal to the best score...
		// then update the best score and also the best move.
		if (Value >= BestScore)
		{
			BestScore = Value;
			BestMove  = TheMove;
		}
	}

	// Retun the best move that was found.
	return BestMove;

};

// Function to return the minimax value of a chessboard.
double GameManager::Minimax(int Depth, double Alpha, double Beta, bool Maximise)
{

	// If the depth is equal to zero then evaluate the state of the board.
	// This provides the condition to ensure recursion doesn't go on forever.
	if (Depth == 0)
	{
		// White wants to maximise the board, whilst black want to minimise.
		if (GetMaxBoardEval()) { return  TheBoard->EvaluateBoard(); }
		else { return -TheBoard->EvaluateBoard(); }
	}

	// Define and set the colour.
	string Colour;
	if (Maximise) { Colour = "White"; }
	else { Colour = "Black"; }

	// Get the vector of all possible moves.
	vector<PossibleMove> GameMoves{ AllPossibleMoves(Colour) };
	// Shuffle the vector to avoid the same moves being played.
	random_shuffle(GameMoves.begin(), GameMoves.end());

	if (Maximise)
	{
		// Initialise the best move value as a very large negative number.
		double BestMove{ -9999 };
		// Iterate through the possible moves.
		for (auto it = GameMoves.begin(); it != GameMoves.end(); it++)
		{
			PossibleMove TheMove = *it;
			// Make the move.
			SpecifiedMove(TheMove.OriginalX, TheMove.OriginalY, TheMove.MovedX, TheMove.MovedY, true);
			// Maximise the best move value.
			BestMove = max(BestMove, Minimax(Depth - 1, Alpha, Beta, !Maximise));
			// Undo that last move.
			UndoAnyNumberOfMoves(1);
			// Use alpha-beta pruning to skip over pointless recursions.
			Alpha = max(Alpha, BestMove);
			if (Beta <= Alpha) { return BestMove; }
		}
		// Return the best move value.
		return BestMove;
	}
	else
	{
		// Initialise the best move value as a very large positive number.
		double BestMove{ 9999 };
		// Iterate through the possible moves.
		for (auto it = GameMoves.begin(); it != GameMoves.end(); it++)
		{
			PossibleMove TheMove = *it;
			// Make the move.
			SpecifiedMove(TheMove.OriginalX, TheMove.OriginalY, TheMove.MovedX, TheMove.MovedY, true);
			// Minimise the best move value.
			BestMove = min(BestMove, Minimax(Depth - 1, Alpha, Beta, !Maximise));
			// Undo that last move.
			UndoAnyNumberOfMoves(1);
			// Use alpha-beta pruning to skip over pointless recursions.
			Beta = min(Beta, BestMove);
			if (Beta <= Alpha) { return BestMove; }
		}
		// Return the best move value.
		return BestMove;
	}

};