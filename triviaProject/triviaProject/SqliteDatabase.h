#pragma once
#include "IDatabase.h"
#include <list>
#include <vector>
#include <io.h>
#include <mutex>
#include "sqlite3.h"

struct User
{
	std::string username;  // username
	std::string password;  // password
	std::string email;  // email
};

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();  // c'tour
	virtual ~SqliteDatabase() = default;  // d'tour

	bool open();  // opens the database
	void close();  // closes the database

	virtual const bool doesUserExist(const std::string username);  // checks if a user exists already
	virtual const bool doesPasswordMatch(const std::string username, const std::string password);
	// checks if a password matches a username
	virtual void addNewUser(const std::string username, const std::string password, const std::string email);
	// adds a new user to the dataBase
	virtual std::list<Question> getQuestions(int amount);  // gets the wanted amount of questions
	virtual float getPlayerAverageAnswerTime(const std::string& username);  // gets the average answer time of a player
	virtual int getNumOfCorrectAnswers(const std::string& username);  // gets the number of correct answers of a player
	virtual int getNumOfTotalAnswers(const std::string& username);  // gets the number of total answers of a player
	virtual int getNumOfPlayerGames(const std::string& username);  // gets the number of games of a player
	virtual std::vector<std::string> getAllUsernames();  // gets all the users

private:
	void insertQes();  // inserts ten quesntions 

	std::string _dbFileName;  // the name of the database file
	sqlite3* _db;  // the database itself
	std::mutex _databaseLock;  // a mutex for the database
	std::unique_lock<std::mutex> _locker;  // a locker for the mutex
};

