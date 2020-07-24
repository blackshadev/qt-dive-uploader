#include "dcdivesampleparser.h"

DiveSampleParser::DiveSampleParser()
{
    currentSample = NULL;
}

DiveSampleParser::~DiveSampleParser()
{

}

void DiveSampleParser::setCallback(sampleparser_callback fn)
{
    callback = fn;
}

void DiveSampleParser::addSample(sampledata_t sample)
{
    if (currentSample != NULL && sample.type == DC_SAMPLE_TIME) {
        callback(currentSample);
    }
    if (sample.type == DC_SAMPLE_TIME) {
        currentSample = new DCDiveSample();
    }

    switch(sample.type) {
        case DC_SAMPLE_EVENT:
        {
            auto event = createEvent(sample.value.event.type, sample.value.event.value);
            currentSample->addEvent(event);
            break;
        }
        case DC_SAMPLE_TIME:
            currentSample->setTime(sample.value.time);
            break;
        case DC_SAMPLE_CNS:
            currentSample->setCNS(sample.value.cns);
            break;
        case DC_SAMPLE_RBT:
            currentSample->setRBT(sample.value.rbt);
            break;
        case DC_SAMPLE_PPO2:
            currentSample->setPPO2(sample.value.ppo2);
            break;
        case DC_SAMPLE_DEPTH:
            currentSample->setDepth(sample.value.depth);
            break;
        case DC_SAMPLE_BEARING:
            currentSample->setBearing(sample.value.bearing);
            break;
        case DC_SAMPLE_SETPOINT:
            currentSample->setSetpoint(sample.value.setpoint);
            break;
        case DC_SAMPLE_HEARTBEAT:
            currentSample->setHeartbeat(sample.value.heartbeat);
            break;
        case DC_SAMPLE_TEMPERATURE:
            currentSample->setTemperature(sample.value.temperature);
            break;
        case DC_SAMPLE_PRESSURE:
        {
            pressures_t pressure;
            pressure.tank = sample.value.pressure.tank;
            pressure.pressure = sample.value.pressure.value;
            currentSample->addPressure(pressure);
            break;
        }
    }

}

void DiveSampleParser::finalize()
{
    if (currentSample != NULL) {
        callback(currentSample);
    }
    currentSample = NULL;
}

DCDiveSampleEvent *DiveSampleParser::createEvent(unsigned int type, int value)
{
    auto event = new DCDiveSampleEvent();
    event->setType(type);
    event->setValue(value);
}
