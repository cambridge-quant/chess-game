// OOP Chess Project: GameManager.h.
// This is the GameManager class header file.
// It contains all the declarations related to the GameManager class.
// James Cummins.

#pragma once
// This will be true only once.
#ifndef MY_CLASS_Game
#define MY_CLASS_Game

// Include the relevant libraries.
#include <iomanip>
#include <time.h>
#include <ctime>
#include <chrono>
#include <map>
#include <fstream>
#include <random>
#include <iterator>
#include <algorithm>
#include <thread>
#include "Board.h"

// Using namespaces.
using namespace BoardNamespace;

// Using a namespace to avoid name collisions.
namespace GameNamespace
{

	// A template function to return a choice of four acceptable values; a, b, c, and d.
	// The fact that it's a template function means the acceptable values could be either integers or chars for example.
	// The template function is unusually (but correctly) declared in the header file.
	template <class T> T GoodInput(T a, T b, T c, T d)
	{

		// An input of type T is declared.
		T Input;
		// The user enters this input.
		cin >> Input;

		// The input is checked against the acceptable values.
		while (toupper(Input) != a && toupper(Input) != b && toupper(Input) != c && toupper(Input) != d)
		{
			// If it was a yes/no question, print this output.
			if (a == 'Y' && b == 'Y' && c == 'N' && d == 'N')
			{
				cerr << "Error: Enter Y for [Y]es, or N for [N]o.\nPlease try again: ";
			}
			// Otherwise print a different output.
			else
			{
				cerr << "Error: Did not recognise input.\nPlease try again: ";
			}

			// Clear the error state.
			cin.clear();
			// Ignore whatever the user typed.
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');;
			// Allow the user to try again.
			cin >> Input;

		}

		// Clear the input buffer.
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		// Return the good input.
		return toupper(Input);

	}

	// Define a PossibleMove variable with four integers describing it.
	struct PossibleMove { int OriginalX, OriginalY, MovedX, MovedY; };

	// GameManager class.
	class GameManager {

	// Private member data.
	private:

		// The chessboard.
		Board *TheBoard;
		// A multimap of the list of moves.
		multimap<int, string> ListOfMoves;
		// A vector of possible moves representing the game.
		vector<PossibleMove> SavedGame;
		// Integers to keep track of how the game develops.
		int GameTurnNumber, CaptureCounter, PawnMoveCounter, GameType;
		// Bool that determines if we want to maximise the board evaluation or not.
		bool MaximiseBoardEvaluation;

	// Public member functions.
	public:

		// Default constructor.
		GameManager();
		// Parameterised constructor.
		GameManager(Board *InputBoard);
		// Destructor.
		~GameManager() {}

		// Access function.
		int  GetGameTurnNumber() const;
		bool GetMaxBoardEval()   const;

		// Function to inform the user that a pawn has been promoted.
		void PrintPawnPromotion();

		// Function to print the list of moves that have happened in the game.
		void PrintList();

		// Function to make a human move.
		void HumanMove();

		// Function to make a computer move.
		void ComputerMove(bool IntelligentTrueRandomFalse);

		// Function to make a specified move.
		void SpecifiedMove(int OldX, int OldY, int NewX, int NewY, bool Save);

		// Function to give the user a choice of game modes.
		int  GameModeSelection();

		// Function to give the user a choice to load a saved game.
		void LoadGameSelection();

		// Function to give the user options before making a move.
		void BeginOptionMenu();

		// Function to give the user options after  making a move.
		void EndOptionMenu();

		// Function to print all the allowed moves.
		void PrintAllowedMoves(string Colour);

		// Function to return the number of allowed moves.
		int  NumberOfAllowedMoves(string Colour);

		// Function to return a vector of all the allowed moves.
		vector<PossibleMove> AllPossibleMoves(string Colour);

		// Function to see if the king is in check mate.
		bool KingInCheckMate(string Colour);

		// Function to check if the fifty move rule has been satisfied.
		bool FiftyMoveRule();

		// Function to check if the game is over or not.
		bool IsGameOver();

		// Function to see if a move can be made without putting the king in check mate.
		bool WillPieceAvoidCheckMate(int OldX, int OldY, int NewX, int NewY, string Colour);

		// Function to undo a move made by the computer.
		void UndoLastComputerMove();

		// Function to undo any number of specified moves.
		void UndoAnyNumberOfMoves(int Number);

		// Function to save the current game.
		void SaveGame();

		// Function to load a saved game.
		void LoadGame();

		// Function to return the best move via the minimax algorithm.
		PossibleMove MinimaxMove(int Depth, bool Maximum);

		// Function to return the minimax value of a chessboard.
		double Minimax(int Depth, double Alpha, double Beta, bool Maximise);

	};

}

#endif