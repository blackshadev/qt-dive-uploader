#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H
#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args );

#endif // STRING_FORMAT_H
