#pragma once

struct RequestInfo
{
	std::string id;  // the id of the request
	time_t receivalTime;  // the time the request was reciecved
	std::vector<unsigned char> buffer;  // the message itself 
};

class IRequestHandler;
struct RequestResult
{
	std::vector<unsigned char> response;  // the response massage
	IRequestHandler* newHandler;  // a pointer to the next handler
};