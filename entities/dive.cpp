#include "dive.h"
#include "../common/vector.h"
#include "../common/datetime.h"

Dive::Dive()
{
    data.fingerprint = NULL;
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

    samples = new std::vector<DiveSample *>();
    tanks = new std::vector<tank_t>();
}

Dive::~Dive()
{
    FREE_VECTOR_PTR(samples)
    FREE_VECTOR(tanks)

}

unsigned char *Dive::getFingerprint()
{
    return data.fingerprint;
}

void Dive::setFingerprint(unsigned char *fp)
{
    data.fingerprint = fp;
}

double Dive::getMaxDepth()
{
    return data.maxDepth;
}

void Dive::setMaxDepth(double depth)
{
    data.maxDepth = depth;
}

unsigned int Dive::getDivetime()
{
    return data.divetime;
}

void Dive::setDivetime(unsigned int time)
{
    data.divetime = time;
}

datetime_t Dive::getDatetime()
{
    return data.datetime;
}

void Dive::setDatetime(datetime_t datetime)
{
    data.datetime = datetime;
}

std::string Dive::getDisplayDateTime()
{
    return format_datetime_iso((dc_datetime_t&)data.datetime);
}

std::string Dive::getISODateTime()
{
    return format_datetime_iso((dc_datetime_t&)data.datetime);
}

optional_t<double> Dive::getMaxTemperature()
{
    return data.maxTemperature;
}

void Dive::setMaxTemperature(double temperature)
{
    data.maxTemperature = temperature;
}

optional_t<double> Dive::getMinTemperature()
{
    return data.minTemperature;
}

void Dive::setMinTemperature(double temperature)
{
        data.minTemperature = temperature;
}

optional_t<double> Dive::getSurfaceTemperature()
{
    return data.surfaceTemperature;
}

void Dive::setSurfaceTemperature(double temperature)
{
    data.surfaceTemperature = temperature;
}

optional_t<double> Dive::getAtmosphericPressure()
{
    return data.atmosphericPressure;
}

void Dive::setAtmosphericPressure(double pressure)
{
    data.atmosphericPressure = pressure;
}

optional_t<salinity_t> Dive::getSalinity()
{
    return data.salinity;
}

void Dive::setSalinity(salinity_t salinity)
{
    data.salinity = salinity;
}

optional_t<divemode_t> Dive::getDivemode()
{
    return data.divemode;
}

void Dive::setDivemode(divemode_t divemode)
{
    data.divemode = divemode;
}

std::vector<tank_t> *Dive::getTanks()
{
    return tanks;
}

std::vector<DiveSample *> *Dive::getSamples()
{
    return samples;
}

void Dive::addTank(tank_t tank)
{
    tanks->push_back(tank);
}

void Dive::addSample(DiveSample *sample)
{
    samples->push_back(sample);
}
