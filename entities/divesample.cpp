#include "divesample.h"
#include "../common/vector.h"

DiveSample::DiveSample()
{
    data.time = 0;
    data.depth.has_value = false;
    data.cns.has_value = false;
    data.rbt.has_value = false;
    data.ppo2.has_value = false;
    data.bearing.has_value = false;
    data.setpoint.has_value = false;
    data.heartbeat.has_value = false;
    data.temperature.has_value = false;

    pressures = new std::vector<pressures_t>();
    events = new std::vector<DiveSampleEvent *>();
}

DiveSample::~DiveSample()
{
    FREE_VECTOR(pressures)
    FREE_VECTOR_PTR(events)
}

unsigned int DiveSample::getTime()
{
    return data.time;
}

void DiveSample::setTime(unsigned int time)
{
    data.time = time;
}

optional_t<double> DiveSample::getDepth()
{
    return data.depth;
}

void DiveSample::setDepth(double depth)
{
    data.depth = depth;
}

optional_t<double> DiveSample::getTemperature()
{
    return data.temperature;
}

void DiveSample::setTemperature(double temperature)
{
    data.temperature = temperature;
}

optional_t<int> DiveSample::getHeartbeat()
{
    return data.heartbeat;
}

void DiveSample::setHeartbeat(int heartbeat)
{
    data.heartbeat = heartbeat;
}

optional_t<int> DiveSample::getBearing()
{
    return data.bearing;
}

void DiveSample::setBearing(int bearing)
{
    data.bearing = bearing;
}

optional_t<double> DiveSample::getCNS()
{
    return data.cns;
}

void DiveSample::setCNS(double cns)
{
    data.cns = cns;
}

optional_t<double> DiveSample::getSetpoint()
{
    return data.setpoint;
}

void DiveSample::setSetpoint(double setpoint)
{
    data.setpoint = setpoint;
}

optional_t<double> DiveSample::getPPO2()
{
    return data.ppo2;
}

void DiveSample::setPPO2(double ppo2)
{
    data.ppo2 = ppo2;
}

std::vector<pressures_t> *DiveSample::getPressures()
{
    return pressures;
}

void DiveSample::addPressure(pressures_t pressure)
{
    pressures->push_back(pressure);
}

std::vector<DiveSampleEvent *> *DiveSample::getEvents()
{
    return events;
}

void DiveSample::addEvent(DiveSampleEvent *sample)
{
    events->push_back(sample);
}
