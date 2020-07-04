#include "dcdivesampleparser.h"

DiveSampleParser::DiveSampleParser()
{
    currentSample = NULL;
}

void DiveSampleParser::setCallback(sampleparser_callback fn)
{
    callback = fn;
}

void DiveSampleParser::addSample(sampledata_t sample)
{
    // todo
}

void DiveSampleParser::finalize()
{

}
