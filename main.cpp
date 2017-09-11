/*This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all user 
interaction.For game logic see the FullCowGame class*/

#pragma once

#include<iostream>
#include<string>
#include "FBullCowGame.h"

//to make syntax Unreal friendly
using Ftext = std::string;
using int32 =int;

//function prototypes as outside the class
void PrintIntro();
void PlayGame();
Ftext GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game, which  we re-use across plays

//entry point of our application
int main() {
	
	bool bPlayAgain = false;
	do {

		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	}

	while (bPlayAgain);



	return 0; //exit application
}



//Introduce the game
void PrintIntro() {
	
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------(oo)        (oo)-------\\ " << std::endl;
	std::cout << "  / | BULL |              | COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

//Plays a single game to completion
void PlayGame(){
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();
	//loop asking for guesses while the game is NOT won
	//and there are still tries ramaining

	while (! BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		Ftext Guess = GetValidGuess(); //

		
		//submit valid guess to the game, and recieve counts

		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
	
		std::cout << "Bulls =" << BullCowCount.Bulls;
		std::cout << " Cows =" << BullCowCount.Cows << "\n\n";
	
	}

	PrintGameSummary();


	return;
}



//loop continually until the user gives a valid guess
Ftext GetValidGuess()
{
	Ftext Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	

	do {
		//get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ": Enter your guess: ";
		getline(std::cin, Guess);


		//Check  status a give feedback
		Status = BCGame.CheckGuessValidity(Guess);

		switch (Status)
		{

		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters. \n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters. \n\n";
			break;
		default:
			break;
			//assume the guess is valid
		}

		
	}
	
	while (Status != EGuessStatus::OK);//keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain() {

	std::cout << "Do you want to play again with the same hidden word (y/n)";
	Ftext Response = "";
	getline(std::cin, Response);
	std::cout << std::endl;
	std::cout << std::endl;

	return (Response[0] == 'y') || (Response[0] == 'Y');
	
}

void PrintGameSummary() 	{

	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n\n";
	}
	else
	{
		std::cout << "BETTER LUCK NEXT TIME\n\n";
	}
}
