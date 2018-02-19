#ifndef DIVE_H
#define DIVE_H
#include <libdivecomputer/parser.h>

template <typename T>
struct optional {
    bool has_value;
    T   value;
};

class Dive
{
public:
    dc_datetime_t datetime;
    unsigned int divetime;
    double maxDepth;
    optional<double> maxTemperature;
    optional<double> minTemperature;
    optional<double> surfaceTemperature;

    Dive(dc_parser_t* parser, const unsigned char* data, size_t size);
};

#endif // DIVE_H
