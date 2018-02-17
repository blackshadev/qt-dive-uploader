#ifndef DIVE_H
#define DIVE_H
#include <libdivecomputer/parser.h>
#include <boost/optional.hpp>

class Dive
{
public:
    dc_datetime_t datetime;
    unsigned int divetime;
    double maxDepth;
    boost::optional<double> maxTemperature;
    boost::optional<double> minTemperature;
    boost::optional<double> surfaceTemperature;

    Dive(dc_parser_t* parser, const unsigned char* data, size_t size);


};

#endif // DIVE_H
