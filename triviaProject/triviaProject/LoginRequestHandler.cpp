#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonPacketSerializer.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* handleFactory)
{
	m_handlerFactory = handleFactory;
	m_loginManager = &(handleFactory->getLoginManager());
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo request)
{
    return request.id == "10000000" || request.id == "10000001";
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo request)
{
	RequestResult result;
	result.newHandler = nullptr;
	if (request.id == "10000000")  // sign up
	{
		return signup(request);

	}
	else if (request.id == "10000001")  // login
	{
		return login(request);
	}

    return result;
}

RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = nullptr;

	LoginRequest loginReq = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);

	bool success = m_loginManager->login(loginReq.username, loginReq.password);  // logging in through loginManager

	result.response = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ 1 });

	if (success) 
	{
		LoggedUser user(loginReq.username);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler(user);
		// if the user logged in, a menu handler will be sent
		return result;
	}
	
	result.response = JsonResponsePacketSerializer::serializeResponse(
		ErrorResponse{ "Couldn't log into this user - the password/username might be incorrect" });
	result.newHandler = this->m_handlerFactory->createLoginRequestHandler();
	// if the user failed to log in, a login handler will be sent
	return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = nullptr;

	SignupRequest signupReq = JsonRequestPacketDeserializer::deserializeSignUpRequest(info.buffer);

	bool success = m_loginManager->signup(signupReq.username, signupReq.password, signupReq.email);  // signing in through loginManager

	result.response = JsonResponsePacketSerializer::serializeResponse(SignupResponse{ 1 });

	if (success)
	{
		LoggedUser user(signupReq.username);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler(user);
		// if the user logged in, a menu handler will be sent
		return result;
	}

	result.newHandler = this->m_handlerFactory->createLoginRequestHandler();
	// if the user failed to log in, a login handler will be sent
	return result;
}
