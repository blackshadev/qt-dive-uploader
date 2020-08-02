#include "exceptions.h"

BaseException::BaseException(const char *msg)
{
    message = msg;
}

BaseException::BaseException(std::string message)
    : BaseException(message.c_str())
{}

const char *BaseException::what() const throw()
{
    return message;
}

ParseException::ParseException(const char *message)
    : BaseException(message)
{

}

ParseException::ParseException(std::string message)
    : BaseException(message)
{

}

NetworkException::NetworkException(const char *message)
    : BaseException(message)
{

}

NetworkException::NetworkException(std::string message)
    : BaseException(message)
{

}
