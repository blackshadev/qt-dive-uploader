#ifndef DIVESAMPLEPARSER_H
#define DIVESAMPLEPARSER_H
#include "../entities/dcdivesample.h"

typedef struct {
    dc_sample_type_t type;
    dc_sample_value_t value;
} sampledata_t;

typedef std::function<void (DCDiveSample *)> sampleparser_callback;

class DiveSampleParser
{
public:
    DiveSampleParser();
    ~DiveSampleParser();
    void setCallback(sampleparser_callback function);
    void addSample(sampledata_t sample);
    void finalize();
private:
    static DCDiveSampleEvent *createEvent(unsigned int type, int value);
    sampleparser_callback callback;
    DCDiveSample *currentSample;
};

#endif // DIVESAMPLEPARSER_H
