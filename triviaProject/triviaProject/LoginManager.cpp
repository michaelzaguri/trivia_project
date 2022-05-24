#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* database)
{
	this->m_database = database;
	_locker = std::unique_lock<std::mutex>(this->_loggedUsersLock, std::defer_lock);  // sets the locker
}

LoginManager::~LoginManager()
{
}

bool LoginManager::signup(const std::string username, const std::string password, const std::string email)
{
	if (!this->m_database->doesUserExist(username))  // signing up only if the users doesnt exist
	{
		_locker.lock();
		this->m_database->addNewUser(username, password, email);
		this->m_loggedUsers.push_back(LoggedUser(username));
		_locker.unlock();
		return true;
	}
	else
	{
		std::cerr << "user already exist" << std::endl;
		return false;
	}
}

bool LoginManager::login(const std::string username, const std::string password)
{
	if (this->m_database->doesPasswordMatch(username, password))  // logs in only if the password matches
	{
		_locker.lock();
		this->m_loggedUsers.push_back(LoggedUser(username));
		_locker.unlock();
		return true;
	}
	else
	{
		return false;
	}
}

bool LoginManager::logout(const std::string username)
{
	bool flag = false;
	if (this->m_database->doesUserExist(username))  // only a user that exists can log out
	{
		_locker.lock();
		auto user = m_loggedUsers.begin();
		while(!flag && user != m_loggedUsers.end())
		{
			if (user->getUsername() == username)  // if the username matches
			{
				auto temp = user;  // saving it as a temp because deleting an iterator will cause problems
				user += 1;
				m_loggedUsers.erase(temp);
				flag = true;
			}
			else
			{
				user++;
			}
		}
		_locker.unlock();
		return true;
	}
	else
	{
		std::cerr << "Cant logout user" << std::endl;
		return false;
	}
}
