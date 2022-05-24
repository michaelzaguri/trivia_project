#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"

class RequestHandlerFactory;

class LoginRequestHandler :
    public IRequestHandler
{
public:
    LoginRequestHandler(RequestHandlerFactory* handleFactory);  // c'tour

    virtual bool isRequestRelevant(RequestInfo);  // checks if the request is relevant to the handler
    virtual RequestResult handleRequest(RequestInfo);  // handles the request and returns the response

private:
    LoginManager* m_loginManager;  // a pointer the loginManager
    RequestHandlerFactory* m_handlerFactory;  // a pointer to the handleFactory

    RequestResult login(const RequestInfo& info);  // logs in a user
    RequestResult signup(const RequestInfo& info);  // signs up a user
};

