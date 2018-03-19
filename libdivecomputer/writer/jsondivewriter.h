#ifndef JSONDIVEWRITER_H
#define JSONDIVEWRITER_H
#include "./divewriter.h"


struct t_computer {
    dc_descriptor_t* descr;
    uint serial;
    uint firmware;
};

class JsonDiveWriter : public DiveWriter
{
public:

    static void write_dive(QJsonObject &target, Dive* dive);
    static void write_tank(QJsonArray &tanksArray, Dive* dive);
    static void write_samples(QJsonArray &tanksArray, Dive* dive);
    static void write_computer(QJsonObject &target, dc_descriptor_t* device_descriptor, uint serial);
};

#endif // JSONDIVEWRITER_H
