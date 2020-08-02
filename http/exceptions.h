#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>

class BaseException : public std::exception
{
public:
    BaseException(const char *message);
    BaseException(std::string message);
    virtual const char *what() const throw() override;
private:
    const char* message;
};


class ParseException : public BaseException
{
public:
    ParseException(const char *message);
    ParseException(std::string message);
};

class NetworkException : public BaseException
{
public:
    NetworkException(const char *message);
    NetworkException(std::string message);
};

#endif // EXCEPTIONS_H
