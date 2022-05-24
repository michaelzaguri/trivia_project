#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* database)
{
	this->m_dataBase = database;
}

std::vector<std::string> StatisticsManager::getHighScore() const
{
	int i = 0;
	float score = 0;
	std::vector<std::string> allUsernames = this->m_dataBase->getAllUsernames();
	std::vector<std::pair<std::string,float>> usersScores;
	// creating a vector that will contain the users and their score
	std::vector<std::string> topFive;
	for (i = 0; i < allUsernames.size(); i++)
	{
		usersScores.push_back(std::make_pair(allUsernames[i], getPlayerScore(allUsernames[i])));
		// getting the score every username and pushing it to the new vector
	}
	std::sort(usersScores.begin(), usersScores.end(), [](auto p1, auto p2) {return p1.second > p2.second;});
	// sorting the vector by the scores from high to low

	for (i = 0; i < TOP_HIGH && i < allUsernames.size(); i++)
	{
		topFive.push_back(usersScores[i].first);  // pushing the top five scores to the vector
	}
	return topFive;
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{
	std::vector<std::string> userStats;
	if (m_dataBase->doesUserExist(username))  // if the user doesn't exist there are no stats
	{
		userStats.push_back(std::to_string(m_dataBase->getNumOfCorrectAnswers(username)));
		userStats.push_back(std::to_string(m_dataBase->getNumOfTotalAnswers(username)));
		userStats.push_back(std::to_string(m_dataBase->getPlayerAverageAnswerTime(username)));
		userStats.push_back(std::to_string(m_dataBase->getNumOfPlayerGames(username)));
	}
	return userStats;
}

float StatisticsManager::getPlayerScore(const std::string& username) const
{
	float avgAnswerTime = this->m_dataBase->getPlayerAverageAnswerTime(username);
	int correctAnswers = this->m_dataBase->getNumOfCorrectAnswers(username);
	float incorrectAnswers = this->m_dataBase->getNumOfTotalAnswers(username) - correctAnswers;
	int games = this->m_dataBase->getNumOfPlayerGames(username);

	if (!incorrectAnswers)  // if there are no incorrect answers, the user gets a bonus
	{
		incorrectAnswers = NO_MISTAKES_BONUS;  // deviding by a number that is between 1 and 0 will make it increase
	}
	
	return ((correctAnswers / incorrectAnswers) * games * SCORE_MUL) / avgAnswerTime;
}


