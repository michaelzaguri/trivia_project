#pragma comment (lib, "ws2_32.lib")
#include "Server.h"
#include "SqliteDatabase.h"

int main(void)
{
    try
    {
        IDatabase* triviaDB = new SqliteDatabase();
        WSAInitializer wsaInit;
        Server myServer(triviaDB);
        myServer.run();

        std::string msg = "";

        while (msg != "EXIT")
        {
            getline(std::cin, msg);
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Error occured: " << e.what() << std::endl;
    }


	return 0;
}