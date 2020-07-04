#include "dcdivesample.h"
#include "../common/vector.h"

DCDiveSample::DCDiveSample()
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
    events = new std::vector<DCDiveSampleEvent *>();
}

DCDiveSample::~DCDiveSample()
{
    FREE_VECTOR(pressures)
    FREE_VECTOR_PTR(events)
}

unsigned int DCDiveSample::getTime()
{
    return data.time;
}

void DCDiveSample::setTime(unsigned int time)
{
    data.time = time;
}

optional_t<double> DCDiveSample::getDepth()
{
    return data.depth;
}

void DCDiveSample::setDepth(double depth)
{
    data.depth = depth;
}

optional_t<double> DCDiveSample::getTemperature()
{
    return data.temperature;
}

void DCDiveSample::setTemperature(double temperature)
{
    data.temperature = temperature;
}

optional_t<int> DCDiveSample::getHeartbeat()
{
    return data.heartbeat;
}

void DCDiveSample::setHeartbeat(int heartbeat)
{
    data.heartbeat = heartbeat;
}

optional_t<int> DCDiveSample::getBearing()
{
    return data.bearing;
}

void DCDiveSample::setBearing(int bearing)
{
    data.bearing = bearing;
}

optional_t<double> DCDiveSample::getCNS()
{
    return data.cns;
}

void DCDiveSample::setCNS(double cns)
{
    data.cns = cns;
}

optional_t<double> DCDiveSample::getSetpoint()
{
    return data.setpoint;
}

void DCDiveSample::setSetpoint(double setpoint)
{
    data.setpoint = setpoint;
}

optional_t<double> DCDiveSample::getPPO2()
{
    return data.ppo2;
}

void DCDiveSample::setPPO2(double ppo2)
{
    data.ppo2 = ppo2;
}

std::vector<pressures_t> *DCDiveSample::getPressures()
{
    return pressures;
}

void DCDiveSample::addPressure(pressures_t pressure)
{
    pressures->push_back(pressure);
}

std::vector<DCDiveSampleEvent *> *DCDiveSample::getEvents()
{
    return events;
}

void DCDiveSample::addEvent(DCDiveSampleEvent *sample)
{
    events->push_back(sample);
}
