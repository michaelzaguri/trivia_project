#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>

struct Question
{
	unsigned int qId;  // the id of the question
	std::string qContent;  // the content of the question
	std::string correctAnswer;  // the corrent answer between the four
	std::vector<std::string> wrongAnswers; // the wrong asnwers between the four
};

class IDatabase
{
public:
	virtual ~IDatabase() = default;  // d'tour

	virtual const bool doesUserExist(const std::string userName) = 0;  // checks if a user exists already
	virtual const bool doesPasswordMatch(const std::string userName, const std::string password) = 0;  
	// checks if a password matches a username
	virtual void addNewUser(const std::string userName, const std::string password, const std::string email) = 0;
	// adds a new user to the dataBase
	virtual std::list<Question> getQuestions(int amount) = 0;  // gets the wanted amount of questions
	virtual float getPlayerAverageAnswerTime(const std::string& username) = 0;  // gets the average answer time of a player
	virtual int getNumOfCorrectAnswers(const std::string& username) = 0;  // gets the number of correct answers of a player
	virtual int getNumOfTotalAnswers(const std::string& username) = 0;  // gets the number of total answers of a player
	virtual int getNumOfPlayerGames(const std::string& username) = 0;  // gets the number of games of a player
	virtual std::vector<std::string> getAllUsernames() = 0;  // gets all the users
};

