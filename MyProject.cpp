// OOP Chess Project: MyProject.cpp.
// This is the main source file.
// It contains structure of how the game is played.
// James Cummins.

// Include the GameManager header file.
#include "GameManager.h"

// Using namespaces.
using namespace GameNamespace;

// Main function
int main()
{

	// Seed the random generator.
	srand(static_cast<unsigned int>(time(NULL)));
	// Initialise the FinishedProgram bool to zero.
	bool FinishedProgram{ false };

	// While the FinishedProgram bool is false, loop over this.
	while (!FinishedProgram)
	{
		// Define the board.
		Board myBoard;
		// Set the colour scheme.
		myBoard.SetColourAndBackground(15, 0);
		// Initialise the board.
		myBoard.InitialiseBoard();
		// Define the game.
		GameManager TheGame(&myBoard);

		// Print useful information.
		cout << "Chess Game by James Cummins." << endl;
		cout << "For maximum gaming pleasure it is recommended to maximise the console screen.\n" << endl;

		// Print the chessboard.
		myBoard.PrintBoard();
		// Select the game mode.
		int GameMode{ TheGame.GameModeSelection() };
		// Give the user an option to load a previously saved game. 
		TheGame.LoadGameSelection();

		// Structure the game according to the game mode selected.
		switch (GameMode) {
		case 1:
			while (!TheGame.IsGameOver()) {
				TheGame.BeginOptionMenu();
				TheGame.HumanMove();
				if (TheGame.IsGameOver()) { break; }
				TheGame.EndOptionMenu();
			}
			break;
		case 2:
			while (!TheGame.IsGameOver()) {
				while (TheGame.GetGameTurnNumber() % 2 == 0) {
					TheGame.BeginOptionMenu();
					TheGame.HumanMove();
					if (TheGame.IsGameOver()) { goto EndCase2; }
					TheGame.EndOptionMenu();
				}
				TheGame.ComputerMove(false);
			}
		EndCase2: break;
		case 3:
			while (!TheGame.IsGameOver()) {
				while (TheGame.GetGameTurnNumber() % 2 == 0) {
					TheGame.BeginOptionMenu();
					TheGame.HumanMove();
					if (TheGame.IsGameOver()) { goto EndCase3; }
					TheGame.EndOptionMenu();
				}
				TheGame.ComputerMove(true);
			}
		EndCase3: break;
		case 4:
			while (!TheGame.IsGameOver()) {
				TheGame.ComputerMove(true);
				if (TheGame.IsGameOver()) { break; }
				TheGame.ComputerMove(false);
			}
			break;
		default:
			cerr << "Error: Game mode not recongised." << endl;
		}

		// Allow the user to view all the moves and save them to a file,
		cout << "\nWould you like to display all the moves and print them to a file (Y/N)? ";
		char Display{ GoodInput('Y', 'Y', 'N', 'N') };
		if (toupper(Display) == 'Y')
		{
			cout << "\nPrinting moves:" << endl;
			TheGame.PrintList();
			cout << "\nMoves printed to file (ChessGame.txt)." << endl;
		}

		// Allow the user to play the chess game again.
		cout << "\nWould you like to use the play another game (Y/N)? ";
		char Repeat{ GoodInput('Y', 'Y', 'N', 'N') };
		if (toupper(Repeat) == 'N') { FinishedProgram = true; }
		else { for (int i = 0; i < 100; i++) { cout << "\n" << endl; } }
	}

	return 0;

}