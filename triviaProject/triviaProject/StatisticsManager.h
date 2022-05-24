#pragma once
#include "IDatabase.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#define SCORE_MUL 100
#define TOP_HIGH 5
#define NO_MISTAKES_BONUS 0.5

class StatisticsManager
{
public:
	StatisticsManager(IDatabase*);  // c'tour

	std::vector<std::string> getHighScore() const;  // returns the top five users
	std::vector<std::string> getUserStatistics(const std::string& username) const;  // gets the stats of a user

private:
	float getPlayerScore(const std::string& username) const;  // gets the score of a player

	IDatabase* m_dataBase;
};

