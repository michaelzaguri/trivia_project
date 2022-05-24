#pragma once
#include <string>

class LoggedUser
{
public:
	LoggedUser() = default;  // c'tour
	LoggedUser(std::string username);  // c'tour

	// getters & setters
	const std::string getUsername();

	bool operator ==(const LoggedUser& other) const; //compares the _serialNumber of those items.
private:
	std::string m_username; // the username of the said user 
};

