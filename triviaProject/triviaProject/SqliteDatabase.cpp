#include "SqliteDatabase.h"
//callback functions

int getUsersData(void* data, int argc, char** argv, char** azColName)
{
	User user;
	std::vector<User>* users = static_cast<std::vector<User>*>(data);
	for (int i = 0; i < argc; i++)  // dividing all the coloums to the diffrent fields
	{
		if (std::string(azColName[i]) == "username")
		{
			user.username = argv[i];
		}
		else if (std::string(azColName[i]) == "password")
		{
			user.password = argv[i];
		}
		else if (std::string(azColName[i]) == "email")
		{
			user.email = argv[i];
			users->push_back(user);
		}
	}
	return 0;
}

int getQuestionsData(void* data, int argc, char** argv, char** azColName)
{
	Question question;
	std::list<Question>* Questions = static_cast<std::list<Question>*>(data);
	for (int i = 0; i < argc; i++)  // dividing all the coloums to the diffrent fields
	{
		if (std::string(azColName[i]) == "Id")
		{
			question.qId = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "Question")
		{
			question.qContent = argv[i];
		}
		else if (std::string(azColName[i]) == "correctAnswer")
		{
			question.correctAnswer = argv[i];
		}
		else if (std::string(azColName[i]) == "firstWrongAnswer")
		{
			question.wrongAnswers.push_back(argv[i]);
			question.wrongAnswers.push_back(argv[i + 1]);
			question.wrongAnswers.push_back(argv[i + 2]);
			Questions->push_back(question);
		}
	}
	return 0;
}

int getStatistic(void* data, int argc, char** argv, char** azColName)
{
	float* statistic = static_cast<float*>(data);
	*statistic = std::stof(argv[0]);
	return 0;
}

//  not callback 
SqliteDatabase::SqliteDatabase()
{
	_locker = std::unique_lock<std::mutex>(_databaseLock, std::defer_lock);
	this->_dbFileName = "trivia_db.sqlite";
	this->open();
}

bool SqliteDatabase::open()
{
	int doesFileExist = _access(_dbFileName.c_str(), 0);
	char* errMessage = nullptr;
	std::string sqlStatement = "CREATE TABLE Users(username TEXT PRIMARY KEY NOT NULL, password TEXT, email TEXT);";
	int res = sqlite3_open(_dbFileName.c_str(), &(_db));
	if (res != SQLITE_OK)  // checking if i can open db or create 1
	{
		_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}
	if (doesFileExist == -1)  //checking if there is already an existing db
	{
		res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
		{
			_db = nullptr;
			std::cout << "Failed to create a table" << std::endl;
			return false;
		}
		sqlStatement = "CREATE TABLE Questions(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Question TEXT, correctAnswer TEXT, firstWrongAnswer TEXT, secondWrongAnswer TEXT, thirdWrongAnswer TEXT);";
		res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
		{
			_db = nullptr;
			std::cout << "Failed to create a table" << std::endl;
			return false;
		}
		this->insertQes();
		sqlStatement = "CREATE TABLE Statistics(username TEXT PRIMARY KEY NOT NULL, averageAnswerTime FLOAT, amountOfCorrectAnswers INTEGER, amountAnswers INTEGER, amountGames INTEGER);";
		res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
		{
			_db = nullptr;
			std::cout << "Failed to create a table" << std::endl;
			return false;
		}
	}
	return true;
}

void SqliteDatabase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}

const bool SqliteDatabase::doesUserExist(const std::string username)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	std::vector<User> user;
	_locker.lock();
	sqlStatement = "SELECT * FROM Users WHERE username = '" + username + "';";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getUsersData, &(user), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		return !user.empty();
	}
	else
	{
		std::cout << "Failed to access DB" << std::endl;
		return false;
	}
}

const bool SqliteDatabase::doesPasswordMatch(const std::string username, const std::string password)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	std::vector<User> user;
	_locker.lock();
	sqlStatement = "SELECT * FROM Users WHERE username = '" + username + "' and password = '" + password + "';";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getUsersData, &(user), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		return !user.empty();
	}
	else
	{
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}
}

void SqliteDatabase::addNewUser(const std::string username, const std::string password, const std::string email)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	_locker.lock();
	sqlStatement = "INSERT INTO Users VALUES('" + username
		+ "', '" + password + "', '"
		+ email + "');";
	res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
	_locker.unlock();
	if (res != SQLITE_OK)
	{
		std::cout << "Failed to add a user" << std::endl;
		return;
	}
}

std::list<Question> SqliteDatabase::getQuestions(int amount)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	std::list<Question> questions;
	_locker.lock();
	sqlStatement = "SELECT * FROM Questions LIMIT " + std::to_string(amount) + ";";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getQuestionsData, &(questions), &errMessage);
	_locker.unlock();
	if (res != SQLITE_OK)
	{
		std::cout << "Failed to access DB" << std::endl;
		questions.clear();// not sure if it will work so i guess ill test it
	}
	return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	float avgTime = 0;
	_locker.lock();
	sqlStatement = "SELECT averageAnswerTime FROM Statistics WHERE username = '" + username + "' LIMIT 1;";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getStatistic, &(avgTime), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		return avgTime;
	}
	else
	{
		std::cout << "Failed to access DB" << std::endl;
		return 0;
	}
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	float CorrectAnswers = 0;
	_locker.lock();
	sqlStatement = "SELECT amountOfCorrectAnswers FROM Statistics WHERE username = '" + username + "' LIMIT 1;";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getStatistic, &(CorrectAnswers), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		return CorrectAnswers;
	}
	else
	{
		std::cout << "Failed to access DB" << std::endl;
		return 0;
	}
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	float totalAnswers = 0;
	_locker.lock();
	sqlStatement = "SELECT amountAnswers FROM Statistics WHERE username = '" + username + "' LIMIT 1;";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getStatistic, &(totalAnswers), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		return totalAnswers;
	}
	else
	{
		std::cout << "Failed to access DB" << std::endl;
		return 0;
	}
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	float totalGames = 0;
	_locker.lock();
	sqlStatement = "SELECT amountGames FROM Statistics WHERE username = '" + username + "' LIMIT 1;";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getStatistic, &(totalGames), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		return totalGames;
	}
	else
	{
		std::cout << "Failed to access DB" << std::endl;
		return 0;
	}
}

std::vector<std::string> SqliteDatabase::getAllUsernames()
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0, i = 0;
	std::vector<User> users;
	std::vector<std::string> usernames;
	_locker.lock();
	sqlStatement = "SELECT * FROM Users;";
	res = sqlite3_exec(_db, sqlStatement.c_str(), getUsersData, &(users), &errMessage);
	_locker.unlock();
	if (res == SQLITE_OK)
	{
		for (i = 0; i < users.size(); i++)
		{
			usernames.push_back(users[i].username);
		}
	}
	else
	{
		std::cout << "Failed to access DB" << std::endl;
	}
	return usernames;
}

void SqliteDatabase::insertQes()
{
	std::string sqlStatement = "";
	char* errMessage = nullptr;
	int res = 0;
	_locker.lock();
	sqlStatement = "INSERT INTO Questions(Question, correctAnswer, firstWrongAnswer, secondWrongAnswer, thirdWrongAnswer) VALUES('Who is the 7th member in the korean girl-group Loona?','JinSoul', 'GoWon', 'Kim Lip', 'HyunJin'), ('Whats the name of Naruto s first teacher?','Iruka', 'Kakashi', 'Sasuke', 'Demon Slayer'), ('Whats the best Magshimim course?', 'Advanced Programming', 'Archi', 'C', 'Networks'), ('How many members are there in Loona?', '12', '8', '10', '11'), ('What Shahar likes about Michael?', 'Nothing!!!', 'His personality', 'His humor', 'His intelligence'), ('What does Michael use the most?', 'Twitter', 'Reddit', 'Instagram', 'Whatsapp'), ('In kpop, NCT stands for:', 'Neo Culture Technology', 'New Chinese Tracks', 'Neo Cyber Technology', 'It doesnt have a meaning'), ('Whos Leo?', 'Shahars cat', 'Shahars dog', 'Michaels cat', 'Michaels cousin'), ('Best Loona solo:', 'Around You', 'They are all good', 'New <3', 'One&Only');";
	res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
	_locker.unlock();
	if (res != SQLITE_OK)
	{
		std::cout << "Failed to insert questions" << std::endl;
		return;
	}
}

