#include "Room.h"

Room::Room()
{
	_locker = std::unique_lock<std::mutex>(this->_usersLock, std::defer_lock);  // sets the locker
}

Room::Room(const RoomData& data)
{
	this->m_metadata = data;
	_locker = std::unique_lock<std::mutex>(this->_usersLock, std::defer_lock);  // sets the locker
}

Room::Room(const Room& data)
{
	_locker = std::unique_lock<std::mutex>(this->_usersLock, std::defer_lock);  // sets the locker
	m_metadata = data.m_metadata;
	m_users = data.m_users;
}

void Room::addUser(LoggedUser newUser)
{
	_locker.lock();
	m_users.push_back(newUser);
	_locker.unlock();
}

void Room::removeUser(LoggedUser removedUser)
{
	_locker.lock();
	m_users.erase(std::find(m_users.begin(), m_users.end(), removedUser));
	_locker.unlock();
}

std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> users;
	std::vector<LoggedUser>::iterator it;

	_locker.lock();
	for (it = m_users.begin(); it != m_users.end(); it++)
	{
		users.push_back(it->getUsername());
	}
	_locker.unlock();
	return users;
}

RoomData Room::getRoomData() const
{
	return this->m_metadata;
}

void Room::setIsActive(const unsigned int& isActive)
{
	_locker.lock();
	this->m_metadata.isActive = isActive;
	_locker.unlock();
}

void Room::setMetadata(const RoomData& roomData)
{
	this->m_metadata = roomData;
}
