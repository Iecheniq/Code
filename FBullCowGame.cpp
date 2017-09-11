#pragma once
#include "FBullCowGame.h"
#include <map>

//to make syntax Unreal friendly
#define TMap std::map 
using int32 = int;

FBullCowGame::FBullCowGame() {Reset();} //default constructor


int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const {return MyHiddenWord.length();} 
bool FBullCowGame::IsGameWon() const {return bGameIsWon;}

int32 FBullCowGame::GetMaxTries() const { 

	TMap<int32, int32>WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7,20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];//we asign a number of tries according to the hidden word length
} 

void FBullCowGame::Reset() {
	
	
	const Fstring HIDDEN_WORD = "planet";//this MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;
	
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}



EGuessStatus FBullCowGame::CheckGuessValidity(Fstring Guess) const {
	
	if (! IsIsogram(Guess))//if guess is not an isogram
	{
		return EGuessStatus::Not_Isogram; 
	}
	else if (!IsLowercase(Guess)) {//if guess is not lowercase
		return EGuessStatus::Not_Lowercase;
	}
	else if (GetHiddenWordLength() != Guess.length()) {//if the guess length is wrong
		return EGuessStatus::Wrong_Length;
	}
	else {
		return EGuessStatus::OK;
	}
	
}

//receives a valid guess, increments  try, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(Fstring Guess) {

	

	MyCurrentTry++;//increment the try number
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length();//assuming same length as guess

	//loop through all letters in the hidden word
	for (int32 GChar = 0; GChar < WordLength; GChar++) {
		//compare letters against the guess
		for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
			//if they match
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				//if they're in the same place
				if (GChar == MHWChar) {
					//increment bulls
					BullCowCount.Bulls++;
				}
				//else
				else {
					//increment cows
					BullCowCount.Cows++;
				}
				
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) {
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(Fstring Word) const {

	//treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool>LetterSeen;//setup a map, TMap is the Unreal std for std::map
	for (auto Letter : Word)// for all letters of the word, called an iterator
	{
		Letter = tolower(Letter); //handle mixcase
		if (LetterSeen[Letter]) { //if the letter is in the map 
			return false; //we do NOT have an isogram
		}

		else {
			LetterSeen[Letter] = true;//add the letter to the map as seen
		}
		
	}

	return true; //for example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(Fstring Word) const {

	for (auto Letter : Word) {

		if (!islower(Letter)) {//if not a lowercase letter 
			return false;
		}
	}
	return true;
}
