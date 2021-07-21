// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct BullCowCount {
	int32 bulls = 0;
	int32 cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;
		virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
		static bool IsIsogram(const FString& guess);
		void ResetGame();
		void ProcessGuess(const FString& guess);
		void FillHiddenWordsArray();
		BullCowCount GetBullCows(const FString& word) const;

		FString hiddenWord_;
		int32 lives_;
		bool gameOver_ = false;
		TArray<FString> availableWords_;
};
