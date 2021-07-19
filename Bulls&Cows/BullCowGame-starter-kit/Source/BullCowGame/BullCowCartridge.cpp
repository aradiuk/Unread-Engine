// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include <iostream>

void UBullCowCartridge::ResetGame()
{
    hiddenWord_ = TEXT("gun");
    lives_ = hiddenWord_.Len();

    PrintLine(TEXT("             Bulls & Cows game."));
    PrintLine(TEXT("Hello there!\nLet us play a game."));
    PrintLine(TEXT("\nTry to guess a %i letter word.\nYou have %i lives."), hiddenWord_.Len(), lives_);
}

bool UBullCowCartridge::IsIsogram(const FString& guess)
{
    bool result = false;

    FString lower = guess.ToLower();
    for (int pos = 0; pos < lower.Len(); ++pos) {
        int newPos = 0;
        lower.FindLastChar(lower[pos], newPos);
        if (newPos != pos) {
            return false;
        }
    }
    return true;
}

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    ResetGame();
}

void UBullCowCartridge::ProcessGuess(const FString& guess)
{
    if (hiddenWord_ == guess) {
        PrintLine(TEXT("Correct! You have won! Press 'Enter' to continue."));
        gameOver_ = true;
    } else {
        if (!(guess.Len() == hiddenWord_.Len())) {
            PrintLine(TEXT("The length is wrong. Try again."));
            return;
        }

        if (!IsIsogram(guess)) {
            PrintLine(TEXT("Not an isogram. Try again."));
            return;
        }

        if (--lives_ == 0) {
            PrintLine(TEXT("Game over. The word was '%s'. Press 'Enter' to continue."), *hiddenWord_);
            gameOver_ = true;
        } else {
            PrintLine(TEXT("Wrong! You have lost a life! %i %s left"), lives_, (lives_ > 1 ? TEXT("lives") : TEXT("life")));
        }
    }
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (gameOver_) {
        gameOver_ = false;
        ClearScreen();
        ResetGame();
        return;
    }

    ProcessGuess(Input);
}