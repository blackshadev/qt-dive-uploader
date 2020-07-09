#include "dcdive.h"
#include "../common/vector.h"
#include "../common/datetime.h"

DCDive::DCDive()
{
    data.fingerprint.data = NULL;
    data.fingerprint.size = 0;
    data.datetime.year = 0;
    data.datetime.month = 0;
    data.datetime.day = 0;
    data.datetime.hour = 0;
    data.datetime.minute = 0;
    data.datetime.second = 0;
    data.datetime.timezone = 0;

    data.divetime = 0;
    data.maxDepth = 0;

    data.salinity.has_value = false;
    data.maxTemperature.has_value = false;
    data.minTemperature.has_value = false;
    data.surfaceTemperature.has_value = false;
    data.atmosphericPressure.has_value = false;
    data.divemode.has_value = false;

    samples = new std::vector<DCDiveSample *>();
    tanks = new std::vector<tank_t>();
}

DCDive::~DCDive()
{
    FREE_VECTOR_PTR(samples)
    FREE_VECTOR(tanks)

}

fingerprint_t DCDive::getFingerprint()
{
    return data.fingerprint;
}

void DCDive::setFingerprint(fingerprint_t fp)
{
    data.fingerprint.data = fp.data;
    data.fingerprint.size = fp.size;
}

double DCDive::getMaxDepth()
{
    return data.maxDepth;
}

void DCDive::setMaxDepth(double depth)
{
    data.maxDepth = depth;
}

unsigned int DCDive::getDivetime()
{
    return data.divetime;
}

void DCDive::setDivetime(unsigned int time)
{
    data.divetime = time;
}

datetime_t DCDive::getDatetime()
{
    return data.datetime;
}

void DCDive::setDatetime(datetime_t datetime)
{
    data.datetime = datetime;
}

std::string DCDive::getDisplayDateTime()
{
    return format_datetime_iso((dc_datetime_t&)data.datetime);
}

std::string DCDive::getISODateTime()
{
    return format_datetime_iso((dc_datetime_t&)data.datetime);
}

optional_t<double> DCDive::getMaxTemperature()
{
    return data.maxTemperature;
}

void DCDive::setMaxTemperature(double temperature)
{
    data.maxTemperature = temperature;
}

optional_t<double> DCDive::getMinTemperature()
{
    return data.minTemperature;
}

void DCDive::setMinTemperature(double temperature)
{
        data.minTemperature = temperature;
}

optional_t<double> DCDive::getSurfaceTemperature()
{
    return data.surfaceTemperature;
}

void DCDive::setSurfaceTemperature(double temperature)
{
    data.surfaceTemperature = temperature;
}

optional_t<double> DCDive::getAtmosphericPressure()
{
    return data.atmosphericPressure;
}

void DCDive::setAtmosphericPressure(double pressure)
{
    data.atmosphericPressure = pressure;
}

optional_t<salinity_t> DCDive::getSalinity()
{
    return data.salinity;
}

void DCDive::setSalinity(salinity_t salinity)
{
    data.salinity = salinity;
}

optional_t<divemode_t> DCDive::getDivemode()
{
    return data.divemode;
}

void DCDive::setDivemode(divemode_t DCDivemode)
{
    data.divemode = DCDivemode;
}

std::vector<tank_t> *DCDive::getTanks()
{
    return tanks;
}

std::vector<DCDiveSample *> *DCDive::getSamples()
{
    return samples;
}

void DCDive::addTank(tank_t tank)
{
    tanks->push_back(tank);
}

void DCDive::addSample(DCDiveSample *sample)
{
    samples->push_back(sample);
}
