// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include <iostream>

const int32 minWordLength = 3;
const int32 maxWordLength = 6;

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
BullCowCount UBullCowCartridge::GetBullCows(const FString& word) const
{
    int32 tmp = 0;
    BullCowCount count;
    for (int32 i = 0; i < word.Len(); ++i) {
        if (word[i] == hiddenWord_[i]) {
            ++count.bulls;
        } else if (hiddenWord_.FindChar(word[i], tmp)) {
            ++count.cows;
        }
    }
}

void UBullCowCartridge::ResetGame()
{
    int32 randomNum = FMath::RandRange(0, availableWords_.Num() - 1);
    hiddenWord_ = availableWords_[randomNum];
    lives_ = hiddenWord_.Len() * 2;

    PrintLine(TEXT("             Bulls & Cows game."));
    PrintLine(TEXT("Hello there!\nLet us play a game."));
    PrintLine(TEXT("\nTry to guess a %i letter word.\nYou have %i lives."), hiddenWord_.Len(), lives_);
}

void UBullCowCartridge::FillHiddenWordsArray()
{
    TArray<FString> words;
    const FString wordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(availableWords_, *wordListPath, [](const FString& word) {
        int32 length = word.Len();
        if (length >= minWordLength && length <= maxWordLength && IsIsogram(word)) {
            return true;
        }
        return false;
    });
}

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FillHiddenWordsArray();

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

            BullCowCount bullCowCount = GetBullCows(guess);
            PrintLine(TEXT("Here are the bulls (%i)  and cows (%i) you've guessed."), bullCowCount.bulls, bullCowCount.cows);
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