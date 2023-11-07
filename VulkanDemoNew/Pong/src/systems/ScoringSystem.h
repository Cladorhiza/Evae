#pragma once

#include <cstdint>
#include <vector>

class ScoringSystem
{
private:

	const uint32_t MAX_SCORE = 3;

	int currentScore;

public:

	ScoringSystem();

	int GetScore() { return currentScore; };
	void SetScore(int score) { currentScore = score; }
};

