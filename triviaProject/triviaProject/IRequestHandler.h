#pragma once
#include <iostream>
#include <time.h>
#include <vector>
#include "Handler.h"

class IRequestHandler
{
public:

	virtual bool isRequestRelevant(RequestInfo) = 0;  // checks if the request is relevant to the handler
	virtual RequestResult handleRequest(RequestInfo) = 0;  // handles the request and returns the response
};

