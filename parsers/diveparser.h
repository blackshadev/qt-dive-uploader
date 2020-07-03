#ifndef DIVEPARSER_H
#define DIVEPARSER_H
#include <libdivecomputer/parser.h>
#include <libdivecomputer/descriptor.h>
#include "../entities/dive.h"

typedef struct {
    dc_device_t *getNative();
} DCDevice;

class DiveParser
{
public:
    DiveParser();
    void setDevice(DCDevice* device);
    Dive *parseDives(const unsigned char* data, unsigned int size);
private:
    dc_parser_t *parser;
};

#endif // DIVEPARSER_H
