#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username)
{
    m_username = username;
}

const std::string LoggedUser::getUsername()
{
    return m_username;
}

bool LoggedUser::operator==(const LoggedUser& other) const
{
    return this->m_username == other.m_username;
}
