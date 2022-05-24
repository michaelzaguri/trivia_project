#pragma once
#include <vector>
#include <string>
#include <mutex>
#include "LoggedUser.h"
#include "IDatabase.h"

class LoginManager
{
public:
	LoginManager(IDatabase* database);  // c'tour
	~LoginManager();  // d'tour

	bool signup(const std::string username, const std::string password, const std::string email);
	// signs up a user to the database
	bool login(const std::string username, const std::string password); // logs in a user
	bool logout(const std::string username);  // logs out a user

private:
	IDatabase* m_database;  // the dataBase
	std::vector<LoggedUser> m_loggedUsers;  // the vector of all the logged users
	std::mutex _loggedUsersLock;  // a mutex for the users vector
	std::unique_lock<std::mutex> _locker;  // a locker for the mutex
};

